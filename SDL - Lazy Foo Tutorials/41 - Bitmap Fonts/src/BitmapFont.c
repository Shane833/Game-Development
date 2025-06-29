#include <BitmapFont.h>
#include <bstrlib.h>
#include <ctype.h>

BitmapFont* BitmapFont_create()
{
	BitmapFont * temp = malloc(sizeof(BitmapFont));
	check_mem(temp); 
	
	temp->font_texture = Texture_create();
	check_mem(temp->font_texture);

	temp->space = 0;
	temp->new_line = 0;

	return temp;
error:
	return NULL;
}

void BitmapFont_free(BitmapFont * bmpf)
{
	Texture_destroy(bmpf->font_texture);
}

bool BitmapFont_buildFont(BitmapFont * bmpf, Window * window, const char * path)
{
	// Get rid of pre-existing texture
	// BitmapFont_free(bmpf);
	
	bool r = Texture_loadPixelsFromFile(bmpf->font_texture, window, path);
	check(r == true, "ERROR : Failed to load file at : %s", path);
	
	// Get the background color
	Uint32 bg_color = Texture_getPixel32(bmpf->font_texture, 0, 0); 
	
	// set cell dimension
	int cell_w = Texture_getWidth(bmpf->font_texture) / 16;
	int cell_h = Texture_getHeight(bmpf->font_texture) / 16;

	// New line variables
	int top = cell_h;
	int base_A = cell_h;
	
	// The current character we're setting
	int current_char = 0;
	
	// Set up the SDL_Rect for each of the characters
	for(int row = 0;row < 16;row++){
		for(int col = 0;col < 16;col++){
			// Set the position of the characters
			bmpf->chars[current_char].x = col * cell_w;
			bmpf->chars[current_char].y = row * cell_h;
			// Set the dimensions of the characters
			bmpf->chars[current_char].w = cell_w;
			bmpf->chars[current_char].h = cell_h;
			// By default the above assignment defines that the glyph spreads
			// across the entire width and height of the cell
			
			// Lets change that by finding the left, top, right and bottom
			// Now we will loop within the cell dimension i.e. cell_w * cell_h
			
			// Find left side, traversing vertically through each column
			// Go through pixel columns
			for(int p_col = 0;p_col < cell_w;p_col++){
				// Go through pixel rows
				for(int p_row = 0;p_row < cell_h;p_row++){
					// Now we are fetching individual pixel within that cell
					int p_x = (cell_w * col) + p_col;
					int p_y = (cell_h * row) + p_row;
					
					// If non colorkey pixel is found
					if( Texture_getPixel32(bmpf->font_texture, p_x, p_y) != bg_color){
						// Set the x offset
						bmpf->chars[current_char].x = p_x;
						
						// breaking the loop by setting the condition false
						p_col = cell_w;
						p_row = cell_h;
					}
				}
			}
			
			// Find the right side, traversing vertically through each column
			// Starting from the right side
			for(int p_col_w = cell_w - 1;p_col_w >= 0;p_col_w--){
				
				for(int p_row_w = 0;p_row_w < cell_h;p_row_w++){
					int p_x = (cell_w * col) + p_col_w;
					int p_y = (cell_h * row) + p_row_w;
					
					// If non keycolor pixel is found
					if(Texture_getPixel32(bmpf->font_texture, p_x, p_y) != bg_color){
						// Set the width
						bmpf->chars[current_char].w = (p_x - bmpf->chars[current_char].x) + 1;
						
						// break the loop
						p_col_w = -1;
						p_row_w = cell_h;
					}
				}
			}

			// Find Top, starting at the top, now first traversing horizontally through each row
			// At the end of this loop, the top will contain the lowest top value among all the glyphs
			for(int p_row = 0; p_row < cell_h; p_row++){
				for(int p_col = 0; p_col < cell_w; p_col++){

					int p_x = (cell_w * col) + p_col;
					int p_y = (cell_h * row) + p_row;
					
					// If non keycolor pixel is found
					if(Texture_getPixel32(bmpf->font_texture, p_x, p_y) != bg_color){
						// If new top is found
						if(p_row < top){
							top = p_row;
						}
						// break the loop
						p_col = cell_w;
						p_row = cell_h;
					}
				}
			}

			// Find Bottom of A
			if(current_char == 'A'){
				// Go through the pixel rows and start from bottom
				// Traversing horizontally through each row
				for(int p_row = cell_h - 1; p_row >= 0; p_row--){
					for(int p_col = 0; p_col < cell_w;p_col++){
						// Get the pixel offsets
						int p_x = (cell_w * col) + p_col;
						int p_y = (cell_h * row) + p_row;

						// If non colorkey pixel is found
						if(Texture_getPixel32( bmpf->font_texture, p_x, p_y) != bg_color){
							// Bottom of A is found
							base_A = p_row;

							// exit the loop
							p_col = cell_w;
							p_row = -1;
						}
					}

				}
			}
			
			// go to the next character
			current_char++;
		}
	}

	// Calculate space
	bmpf->space = cell_w / 2;

	// Calculate new line
	bmpf->new_line = base_A - top; // here top is the lowest value of top pixel among all the glyphs

	// lop off excess top pixels
	for(int i = 0;i < 256;i++){
		bmpf->chars[i].y += top; // bring down the y coordinate
		bmpf->chars[i].h -= top; // and increasing the height such that all characters look equal on new lines
	}

	// Create the final texture
	check(Texture_loadFromPixels(bmpf->font_texture, window) == true,"Failed to create Texture from Pixels!");

	return true;
error:
	return false;
}


void BitmapFont_render(BitmapFont * bmpf, Window * window, int x, int y, const char * text)
{
	// If the font has been build
	if(Texture_getWidth(bmpf->font_texture) > 0){
		// Temp offsets
		int cur_x = x, cur_y = y;

		// Convert the string to bstring
		bstring str = bfromcstr(text);
		check(str != NULL, "ERROR : Failed to create String!");

		// Go through the text
		for(int i = 0;i < blength(str);i++){
			// If the current character is space
			if( bdata(str)[i] == ' ' ){
				// Move over
				cur_x += bmpf->space;
			}
			// If the current character is newline
			else if( bdata(str)[i] == '\n'){
				// Move down
				cur_y += bmpf->new_line;
				// Move back
				cur_x = x;
			}
			else{
				// Get the ascii value of the character
				int ascii = (unsigned char)(bdata(str)[i]);
				// show the character
				Texture_render(bmpf->font_texture, window, cur_x, cur_y, &(bmpf->chars[ascii]));
				// Move over the width of the character with one pixel of padding
				cur_x += bmpf->chars[ascii].w + 1;
			}
		}
		// free the string
		bdestroy(str);
	}

error: // fallthrough
	return;
}