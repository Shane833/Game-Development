#include <BitmapFont.h>

BitmapFont* BitmapFont_create()
{
	BitmapFont * temp = malloc(sizeof(BitmapFont));
	check_mem(temp); 
	
	temp->space = 0;
	temp->new_line = 0;

	return temp;
error:
	return NULL;
}

bool BitmapFont_buildFont(BitmapFont * bmpf, Window * window, const char * path)
{
	// Get rid of pre-existing texture
	// BitmapFont_free(bmpf);
	
	bool r = Texture_loadPixelsFromFile(&(bmpf->texture), window, path);
	check(r == true, "ERROR : Failed to load file at : %s", path);
	
	// Get the background color
	Uint32 bg_color = Texture_getPixel32(&(bmpf->texture), 0, 0); 
	
	// set cell dimension
	int cell_w = Texture_getWidth(&(bmpf->texture)) / 16;
	int cell_h = Texture_getHeight(&(bmpf->texture)) / 16;
	
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
			
			// Find left side
			// Go through pixel columns
			for(int p_col = 0;p_col < cell_w;p_col++){
				// Go through pixel rows
				for(int p_row = 0;p_row < cell_h;p_row++){
					// Now we are fetching individual pixel within that cell
					int p_x = (cell_w * col) + p_col;
					int p_y = (cell_h * row) + p_row;
					
					// If non colorkey pixel is found
					if( Texture_getPixel32(&(bmpf->texture), p_x, p_y) != bg_color){
						// Set the x offset
						bmpf->chars[current_char].x = p_x;
						
						// breaking the loop by setting the condition false
						p_col = cell_w;
						p_row = cell_h;
					}
				}
			}
			
			// Find the right side
			// Starting from the right side
			for(int p_col_w = cell_w - 1;p_col_w >= 0;p_col_w--){
				
				for(int p_row_w = 0;p_row_w < cell_h;p_row_w++){
					int p_x = (cell_w * col) + p_col_w;
					int p_y = (cell_h * row) + p_row_w;
					
					// If non keycolor pixel is found
					if(Texture_getPixel32(&(bmpf->texture), p_x, p_y) != bg_color){
						// Set the width
						bmpf->chars[current_char].w = (p_x - bmpf->chars[current_char].x) + 1;
						
						// break the loop
						p_col_w = -1;
						p_row_w = cell_h;
					}
				}
			}
			
			
		}
	}
	
}
