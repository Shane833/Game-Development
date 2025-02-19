// In this program we will change the 
#include <SDL2/SDL.h>
#include <dbg.h>
#include <stdbool.h>

// Usual globals
SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// usual functions
bool init();
bool loadMedia();
void close();

// New variables
// An enum to hold unique ID to the differnt surfaces/images
enum keypressSurfaces{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL // Since the values starts at 0, so last element would be the total no. of elemets
};
// This will hold the different surfaces to be displayed
// basically an array of SDL_Surface*
SDL_Surface* key_press_Surfaces[KEY_PRESS_SURFACE_TOTAL] = { NULL }; // Personal naming style for struct and classes (First letter Uppercase)
// Current Surface to be drawn to the window						 // For variables of containers like enum, arrays, hashmaps, lists (underscores with last letter capital)
SDL_Surface* current_surface = NULL;								 // For function not spaces, first letter always small, followed by capital letters

// New Function
// This function will load up a surface from a file
// and return the pointer to that surface
SDL_Surface* loadSurface(const char* filename);

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
				
				case SDL_KEYDOWN: // This event corresponds to the user pressing down a key
					switch(e.key.keysym.sym){ // Now we check for key that pressed (represented by keysym and its corresponding code is in sym)
						
						case SDLK_UP:
							current_surface = key_press_Surfaces[KEY_PRESS_SURFACE_UP];
							break;
						
						case SDLK_DOWN:
							current_surface = key_press_Surfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						
						case SDLK_LEFT:
							current_surface = key_press_Surfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						
						case SDLK_RIGHT:
							current_surface = key_press_Surfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						
						default:
							current_surface = key_press_Surfaces[KEY_PRESS_SURFACE_DEFAULT];
					}
				break;
			}
		}
		// Blit the surface and update it
		SDL_BlitSurface(current_surface, NULL, screen_surface, NULL);
			
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
	check(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Key Presses",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create window! SDL_Error: %s\n", SDL_GetError());
	
	screen_surface = SDL_GetWindowSurface(window);
	
	return true;
	error:
		return false;
}

SDL_Surface* loadSurface(const char* filename){
	SDL_Surface* temp = SDL_LoadBMP(filename);
	check(temp != NULL, "Failed to load the file: %s, SDL_Error: %s\n", filename, SDL_GetError());
	return temp;
	error:
		return NULL;
}

bool loadMedia(){
	// Load default and key based surfaces
	key_press_Surfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Assets/1.bmp");
	check(key_press_Surfaces[KEY_PRESS_SURFACE_DEFAULT] != NULL, "Failed to load! SDL_Error: %s\n", SDL_GetError());
	
	key_press_Surfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Assets/1.bmp");
	check(key_press_Surfaces[KEY_PRESS_SURFACE_UP] != NULL, "Failed to load! SDL_Error: %s\n", SDL_GetError());
	
	key_press_Surfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Assets/2.bmp");
	check(key_press_Surfaces[KEY_PRESS_SURFACE_DOWN] != NULL, "Failed to load! SDL_Error: %s\n", SDL_GetError());
	
	key_press_Surfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Assets/3.bmp");
	check(key_press_Surfaces[KEY_PRESS_SURFACE_LEFT] != NULL, "Failed to load! SDL_Error: %s\n", SDL_GetError());
	
	key_press_Surfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Assets/4.bmp");
	check(key_press_Surfaces[KEY_PRESS_SURFACE_RIGHT] != NULL, "Failed to load! SDL_Error: %s\n", SDL_GetError());
	
	return true;
	error:
		return false;
}

void close(){
	SDL_FreeSurface(current_surface);
	current_surface = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	
	SDL_Quit();
}