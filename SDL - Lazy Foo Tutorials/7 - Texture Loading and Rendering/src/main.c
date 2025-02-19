#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <stdbool.h>
#include <dbg.h>

// Globals
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; // Renderer we'll use
SDL_Texture* current_texture = NULL; // Texture to hold the images

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Functions
SDL_Texture* loadTexture(const char* filename); // Loads the textures from the images
bool init();
bool loadMedia();
void close();


int main(int argc, char* argv[]){
	
	bool r = init();
	check(r == true, "Something went wrong!");
	
	r = loadMedia();
	check(r == true, "Something went wrong!");
	
	bool quit = false;
	SDL_Event e;
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT) quit = true;
		}
		
		SDL_RenderClear(renderer); // Clears the screen	
		SDL_RenderCopy(renderer,current_texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	
	close();
	
	return 0;
	error:
		return 1;
}

// Function Definitions
bool init(){
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0,"Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Texture Loading and Rendering",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create the window! SDL_Error: %s\n", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
	// we pass the window, the index of the driver (or -1 to use the first one you can find that can fulfill these requirements)
	// the last argument is the flag to choose among different rendering technique (i.e done by the hardware or the software)
	check(renderer != NULL, "Failed to create the renderer! SDL_Error: %s\n", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255 , 255, 255); // we provide the initial color of the renderer
	// first arg is the renderer itself followed by RGBA values
	
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_Image! IMG_Error: %s\n", IMG_GetError());
	
	return true;
	error:
		return false;
}

SDL_Texture* loadTexture(const char* filename){
	SDL_Texture* final_texture = NULL;
	
	SDL_Surface* loaded_surface = IMG_Load(filename);
	check(loaded_surface != NULL, "Failed to load the image: %s ---- IMG_Error: %s\n", filename, IMG_GetError());
	
	final_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface); // Converts the surface pixels to a texture
	check(final_texture != NULL, "Failed to create a texture from image: %s ---- SDL_Error: %s\n", filename, SDL_GetError());
	
	SDL_FreeSurface(loaded_surface);
	
	return final_texture;
	error:
		return NULL;
}

bool loadMedia(){
	current_texture = loadTexture("Assets/1.png");
	check(current_texture != NULL, "Failed to load the texture!");
	
	return true;
	error:
		return false;
}

void close(){
	SDL_DestroyTexture(current_texture);
	current_texture = NULL;
	
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	SDL_Quit();
	IMG_Quit();
}
