#include <Game.h>

static bool running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool init(const char* title, int xpos, int ypos, int width, int height, int flags){

    if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    }
    if(window){
        renderer = SDL_CreateRenderer(window,-1,0);
    }
    if(renderer){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        running = true;
		
		return true;
    }
	
	return false;
}

void handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    
	switch(event.type){
		case SDL_QUIT:
			running = false;
			break;
		default:
			break;
	}
}	

void render(){
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool isRunning(){
	return running;
}