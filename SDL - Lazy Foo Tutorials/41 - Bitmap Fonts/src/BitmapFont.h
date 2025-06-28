#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include <Texture.h>

typedef struct{
	Texture * font_texture;
	SDL_Rect chars[256]; // Contains the coordinates of all the characaters
	int space;
	int new_line; // spacing variables
}BitmapFont;

BitmapFont* BitmapFont_create();
bool BitmapFont_buildFont(BitmapFont * bmpf, Window * window, const char * path); // Loads up the font from a file
void BitmapFont_free(BitmapFont * bmpf); // frees the loaded up texture
void BitmapFont_render(BitmapFont * bmpf, Window * window, int x, int y, const char * text); // display the text 
void BitmapFont_destroy(BitmapFont * bmpf); // deallocates memory
#endif
