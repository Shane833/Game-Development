#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dbg.h>
// We will use clip rendering to only render a part of the texture as opposed to the whole thing


// Texture Data Structure
typedef struct Texture{
	SDL_Texture* texture; // Holds the SDL_Texture
	size_t width; // Defines the width and height and used size_t such that it remains positive
	size_t height;
}Texture;

// Functions
Texture* Texture_create(); // Initializes the variables
bool Texture_loadFromFile(Texture* texture, const char* filepath); // Loads an image from a specified path
void Texture_render(Texture* texture, int x, int y, SDL_Rect *clip); // Renders texture at a given point and only a given part
size_t Texture_getWidth(Texture* texture); // Returns the image dimensions
size_t Texture_getHeight(Texture* texture);
void Texture_destroy(Texture* texture);

// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
// Scene Textures and rectangle
SDL_Rect sprite_Clips[4];
Texture sprite_sheet;

// Usual Functions
bool init();
bool loadMedia();
void close();

int main(int argc, char* argv[])
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	bool quit = false;
	SDL_Event e;
	
	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); // Clears the current frame
		
		Texture_render(&sprite_sheet, 0, 0, &sprite_Clips[0]); // Render the clipped textures
		Texture_render(&sprite_sheet, SCREEN_WIDTH - sprite_Clips[1].w, 0, &sprite_Clips[1]);
		Texture_render(&sprite_sheet, 0, SCREEN_HEIGHT - sprite_Clips[2].h, &sprite_Clips[2]);
		Texture_render(&sprite_sheet, SCREEN_WIDTH - sprite_Clips[3].w, SCREEN_HEIGHT - sprite_Clips[3].h, &sprite_Clips[3]);
		
		SDL_RenderPresent(renderer); // Display the frame to the screen
	}
	
	close();
	
	return 0;
	error:
		return 1;
}

// Texture Functions 
// Initializes the variables to their equivalent zeroes
Texture* Texture_create()
{
	Texture* result = calloc(1, sizeof(Texture));
	check(result != NULL, "Failed to create a custom Texture");
	
	result->texture = NULL;
	result->width = 0;
	result->height = 0;
	
	return result;
	error:
		return NULL;
}

void Texture_destroy(Texture* texture)
{
	// Only getting rid of the SDL_Texture
	SDL_DestroyTexture(texture->texture);
	texture->texture = NULL;
	texture->width = 0;
	texture->height = 0;
}

bool Texture_loadFromFile(Texture* texture, const char* filepath)
{
	// First we deallocate the texture if there is already one loaded in it
	Texture_destroy(texture);
	
	// The final texture
	SDL_Texture* new_texture = NULL;
	
	// Load the image at the specified path
	SDL_Surface* loaded_surface = IMG_Load(filepath);
	check(loaded_surface != NULL, "Failed to load the surface from %s! IMG_Error : %s\n", filepath, IMG_GetError());
	
	// Color keying the image
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 255, 255));
	// Here we first provide the surface we want to color key, the we let SDL know that we want to enable
	// color key and the last is the pixel we want to color key with
	// Also the most cross platform way to create a pixel from the RGB is by using the SDL_MapRGB() function
	// First we provide the format and then the corresponding RGB values
	
	// Now to create a texture from surface pixels
	new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	check(new_texture != NULL, "Failed to convert surface of %s to a texture! SDL_Error : %s\n", filepath, SDL_GetError());
	
	// Get the image dimension
	texture->width = loaded_surface->w;
	texture->height = loaded_surface->h;
	
	// Get rid of the old surface
	SDL_FreeSurface(loaded_surface);
	
	texture->texture = new_texture;
	check(texture->texture != NULL, "Failed to copy the new texture of %s! SDL_Error: %s\n", filepath, SDL_GetError());
	
	return true;
	error:
		return false;
}

void Texture_render(Texture* texture, int x, int y, SDL_Rect* clip)
{
	// Here we will define the position as to where we want to display our texture
	// First create a SDL_Rect for defining the dimension and position
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	
	check_debug(clip != NULL, "The provided clip is NULL! Rendering without it");
	
	// We only provide the area we want to render using the clip 
	// and we give that info to the render_quad
	render_quad.w = clip->w; 
	render_quad.h = clip->h;
	
	SDL_RenderCopy(renderer, texture->texture, clip, &render_quad); // Here the new argumet is the source rect which must be provided
	return;
	// Don't forget to add the return before this as it will execute as a normal flow of the program
	error:
		SDL_RenderCopy(renderer, texture->texture, NULL, &render_quad); // This is in the case the clip is not provided 
}

size_t Texture_getWidth(Texture* texture)
{
	return texture->width;
}

size_t Texture_getHeight(Texture* texture)
{
	return texture->height;
}

// General functions

bool init()
{
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Color Keying",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s\n", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s\n", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_Image! IMG_Error: %s\n", IMG_GetError());
	
	return true;
	error:
		return false;
}

bool loadMedia()
{
	bool r = Texture_loadFromFile(&sprite_sheet, "Assets/dots.png");
	check(r == true, "Failed to load the GUY!");
	
	// Now we define the rects for individual dots
	sprite_Clips[0].x = 0;
	sprite_Clips[0].y = 0;
	sprite_Clips[0].w = 100;
	sprite_Clips[0].h = 100;
	
	sprite_Clips[1].x = 100;
	sprite_Clips[1].y = 0;
	sprite_Clips[1].w = 100;
	sprite_Clips[1].h = 100;
	
	sprite_Clips[2].x = 0;
	sprite_Clips[2].y = 100;
	sprite_Clips[2].w = 100;
	sprite_Clips[2].h = 100;
	
	sprite_Clips[3].x = 100;
	sprite_Clips[3].y = 100;
	sprite_Clips[3].w = 100;
	sprite_Clips[3].h = 100;
	
	return true;
	error:
		return false;
}

void close()
{
	Texture_destroy(&sprite_sheet);
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	IMG_Quit();
	SDL_Quit();
}