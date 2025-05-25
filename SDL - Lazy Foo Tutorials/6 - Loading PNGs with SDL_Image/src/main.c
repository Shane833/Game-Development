#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dbg.h>
#include <stdbool.h>

/*
// Usual globals
SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Surface* current_surface = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// usual functions
bool init();
bool loadMedia();
void close();
// TEMP SDL_Surface* loadSurface(const char* filename);

// Main Function
int main(int argc, char* argv[]){
	bool r = init();
	check(r == true, "Something went wrong!\n");
	
	r = loadMedia();
	check(r == true, "Something went wrong!\n");
	
	bool quit = false;
	SDL_Event e;
	
	while(!quit){
		// Handling events
		while(SDL_PollEvent(&e) != 0){
			
			switch(e.type){
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
		// Now we will apply the surface to the screen
		// TEMP SDL_BlitSurface(current_surface, NULL, screen_surface, NULL);
			
		// Update it
		SDL_UpdateWindowSurface(window);
	}
						
	close();
	
	return 0;
	error:
		return 1;

}

// Function definitions
bool init(){
	// See when you load a bitmap, it's typically loaded in a 24bit format since most bitmaps are 24bit. 
	// Most, if not all, modern displays are not 24bit by default. If we blit an image that's 24bit onto 
	// 32bit image, SDL will convert it every single time the image is blitted.
	
	check(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Loading PNGs",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create window! SDL_Error: %s\n", SDL_GetError());
	
	// initializing the SDL_Image subsystem
	// printf("IMG_INIT_PNG: %d\n",IMG_INIT_PNG);
	// printf("IMG_Init(): %d\n", IMG_Init(IMG_INIT_PNG));
	// printf("IMG_Init() & IMG_INIT_PNG: %d\n", !IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG);
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Image Subsystem Initialization failed!, IMG_Error: %s\n", IMG_GetError());
	
	// TEMP screen_surface = SDL_GetWindowSurface(window);
	
	return true;
	error:
		return false;
}

// TEMP
SDL_Surface* loadSurface(const char* filename){
	SDL_Surface* optimized_surface = NULL;
	
	// We will now use the IMG_Load() method to load our images
	SDL_Surface* loaded_surface = IMG_Load(filename);
	check(loaded_surface != NULL, "Failed to load the image: %s, IMG_Error: %s\n", filename, IMG_GetError());
	
	optimized_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);
	check(optimized_surface != NULL, "Failed to convert surface! SDL_Error: %s\n", SDL_GetError());
	
	SDL_FreeSurface(loaded_surface);
	
	return optimized_surface;
	error:
		return NULL;
}
// uptill here

bool loadMedia(){
	// TEMP
	current_surface = loadSurface("Assets/1.png");
	check(current_surface != NULL, "Failed to load the image!");
	// uptill here

	return true;
	error:
		return false;
}

void close(){
	// TEMP
	SDL_FreeSurface(current_surface);
	current_surface = NULL;
	// uptill here

	SDL_DestroyWindow(window);
	window = NULL;
	
	// Closing the IMG subsystem too
	IMG_Quit();
	SDL_Quit();
}
*/

// To be used with Linux
SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Surface* current_surface = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Loading PNGs",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create window! SDL_Error: %s\n", SDL_GetError());
	
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Image Subsystem Initialization failed!, IMG_Error: %s\n", IMG_GetError());
	
	screen_surface = SDL_GetWindowSurface(window);

	return true;
error:
	return false;
}

bool loadMedia()
{
	// We will now use the IMG_Load() method to load our images
	SDL_Surface* loaded_surface = IMG_Load("Assets/1.png");
	check(loaded_surface != NULL, "Failed to load the image: 1.png, IMG_Error: %s\n", IMG_GetError());
	
	current_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);
	check(current_surface != NULL, "Failed to convert surface! SDL_Error: %s\n", SDL_GetError());
	
	SDL_FreeSurface(loaded_surface);

	return true;
error:
	return false;
}

void handleEvents()
{
	bool quit = false;
	SDL_Event e;
	
	while(!quit){
		// Handling events
		while(SDL_PollEvent(&e) != 0){
			switch(e.type){
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
		// We need to put the following code into a loop since it will run every frame

		// Now we will apply the surface to the screen
		SDL_BlitSurface(current_surface, NULL, screen_surface, NULL);
			
		// Update it
		SDL_UpdateWindowSurface(window);	
	}
}

void close()
{
	

	
}

int main(int argc, char* argv[])
{
	init();
	
	loadMedia();
	
	handleEvents();
	

	// Ok so according to this convention
	// everything should be closed/deallocated in the
	// main function itself and you can put other things anywhere you wish
	SDL_FreeSurface(current_surface);
	current_surface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	// Closing the IMG subsystem too
	IMG_Quit();
	SDL_Quit();
}


