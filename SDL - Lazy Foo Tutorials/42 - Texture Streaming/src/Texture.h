#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <Window.h>
#include <stdbool.h>
#include <math.h>
#include <dbg.h>

// Texture Data Structure
typedef struct Texture{
	/* Texture Members */
	SDL_Texture * texture; 
	SDL_Surface * pixels_surface; // Holds the pixel data of the image

	void * raw_pixels; // used to store the data until we don't send it to the gpu
	int raw_pitch;

	size_t width; 
	size_t height;
}Texture;

// Functions
Texture* Texture_create(); // Initializes the variables
bool Texture_createBlank(Texture * texture, Window * window, size_t width, size_t height); // Creates a blank texture of desired dimensions so that we can stream data to it

bool Texture_loadFromFile(Texture * texture, Window * window, const char * filepath); // Loads an image from a specified path
bool Texture_loadPixelsFromFile(Texture * texture, Window * window, const char * filepath); // Loads the image into pixel buffer
bool Texture_loadFromPixels(Texture * texture, Window * window); // Creates an image from pre-loaded textures
bool Texture_loadFromRenderedText(Texture * texture, Window * window, TTF_Font * font, const char * text, SDL_Color text_color); // This function will return images based on the font and the color and obviosuly the text too duh!

size_t Texture_getWidth(Texture * texture); // Returns the image width
size_t Texture_getHeight(Texture * texture); // Returns the image height
Uint32* Texture_getPixels32(Texture * texture); // Returns the raw pixels of the image
Uint32 Texture_getPixel32(Texture * texture, Uint32 x, Uint32 y); // Returns a pixel at (x,y) coordinates
Uint32 Texture_getPitch32(Texture * texture); // Pitch is the width of texture in memory, it can be of different sizes using pitch we can determine how it is in memory

Uint32 Texture_mapRGBA(Texture * texture, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha); // This returns a single 32-bit pixel in the format of internal SDL_Surface 
void Texture_setColor(Texture * texture, Uint8 red, Uint8 green, Uint8 blue); // Set the color modulation of the texture
void Texture_setBlendMode(Texture * texture, SDL_BlendMode blending); // Set the different blending mode on a texture
void Texture_setAlpha(Texture * texture, Uint8 alpha); // Modulates the alpha values
void Texture_copyRawPixels32(Texture * texture, void * pixels); // copies the raw pixels data that we want to stream

bool Texture_lockTexture(Texture * texture); // gets the pointer to which we wish to send pixels to 
bool Texture_unlockTexture(Texture * texture); // uploads pixel data to the pixels

void Texture_render(Texture * texture, Window * window, int x, int y, SDL_Rect * clip); // Renders texture at a given point and only a given part
void Texture_renderEx(Texture * texture, Window * window, int x, int y, SDL_Rect * clip, double angle, SDL_Point * center, SDL_RendererFlip flip); // Provides extra functionality while rendering

void Texture_destroy(Texture * texture); // Deallocates the memory from the texture

#endif