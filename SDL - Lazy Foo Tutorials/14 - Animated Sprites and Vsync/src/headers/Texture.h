#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dbg.h>
#include <stdbool.h>

// Texture Data Structure
typedef struct{
	SDL_Texture* texture; // Holds the SDL_Texture
	size_t width; // Defines the width and height and used size_t such that it remains positive
	size_t height;
}Texture;

// Functions
Texture* Texture_create(); // Initializes the variables
bool Texture_loadFromFile(SDL_Renderer* renderer, Texture* texture, const char* filepath); // Loads an image from a specified path
void Texture_render(SDL_Renderer* renderer, Texture* texture, int x, int y, SDL_Rect *clip); // Renders texture at a given point and only a given part
size_t Texture_getWidth(Texture* texture); // Returns the image width
size_t Texture_getHeight(Texture* texture); // Returns the image height
void Texture_setColor(Texture* texture, Uint8 red, Uint8 green, Uint8 blue); // Set the color modulation of the texture
void Texture_setBlendMode(Texture* texture, SDL_BlendMode blending); // Set the different blending mode on a texture
void Texture_setAlpha(Texture* texture, Uint8 alpha); // Modulates the alpha values
void Texture_destroy(Texture* texture); // Deallocates the memory from the texture

#endif