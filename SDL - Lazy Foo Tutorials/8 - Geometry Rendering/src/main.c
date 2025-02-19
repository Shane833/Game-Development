#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

// Globals
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Functions
bool init();
void close();

int main(int argc, char* argv[]){
	
	bool r = init();
	check(r == true, "Initialization failed!");
	
	bool quit = false;
	SDL_Event e;
	
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
		
		// We manually clear the frames everytime
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Define it for the initial frame
		SDL_RenderClear(renderer);
		
		// The SDL_Rect struct contatins the x, y, w and h members which are to be initialized
		SDL_Rect fill_rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Color for the filled rectangle
		SDL_RenderFillRect(renderer, &fill_rect); // Fills the rectangle up with colors
		
		SDL_Rect outline_rect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Color for the outline rectangle
		SDL_RenderDrawRect(renderer, &outline_rect);
		
		// Drawing a line
		SDL_SetRenderDrawColor(renderer, 0, 0 , 255, 255);
		SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
		// We define the starting and ending x and y coordinates
		
		// Drawing a dotted line
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		for(int i = 0;i < SCREEN_HEIGHT; i += 4){
			SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i); // provide with the x and y positions
		}
		
		SDL_RenderPresent(renderer); // Update the screens
	}
	
	close();
	
	return 0;
	error:
		return 1;
}

// Function definition
bool init(){
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Geometry Rendering",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create the window! SDL_Error: %s\n", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	check(renderer != NULL, "Failed to create the renderer! SDL_Error: %s\n", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	return true;
	error:
		return false;
}

void close(){
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	SDL_Quit();
}