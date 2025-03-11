#include <SDL2/SDL.h>
#include <stdbool.h>
#include "dbg.h"

/*
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* tree = NULL;

bool init(); 
bool loadMedia();
void close();


int main(int argc, char* argv[]){

	bool r = init();
	//check(r == true, "Something went wrong!!!");
	
	r = loadMedia();
	// check(r == true, "Something went wrong!!!");
	
	SDL_BlitSurface(tree, NULL, screenSurface, NULL);
	
	SDL_UpdateWindowSurface(window);
	
	SDL_Event e;
	bool quit = false;
	
	while(quit == false){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
	}
	
	close();
	
return 0;
error:
	return 1;
}

bool init(){ 
	// check(SDL_Init(SDL_INIT_VIDEO) >= 0,"Failed to initialize SDL! SDL_Error : %s\n", SDL_GetError());
	if(SDL_Init(SDL_INIT_VIDEO) < 0) printf("Failed to initialize SDL\n");
	window = SDL_CreateWindow("Getting an Image on the Screen",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);
	// check(window != NULL, "Failed to create the window! SDL_Error: %s", SDL_GetError());
	
	screenSurface = SDL_GetWindowSurface(window);
	
	return true;
	//error:
		//return false;
}

bool loadMedia(){ 
	tree = SDL_LoadBMP("Assets/tree.bmp");
	//check(tree != NULL,"Falied to load tree.bmp! SDL_Error: %s\n", SDL_GetError());
	
	return true;
	//error:
	//	return false;
}

void close(){
	SDL_FreeSurface(tree);
	tree = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	
	SDL_Quit();
}
*/

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* tree = NULL;

bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL!");
	
	window = SDL_CreateWindow("Getting an Image on the Screen",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);
	
	check(window != NULL, "Failed to initialize window!");
	
	screenSurface = SDL_GetWindowSurface(window);
	
	return true;
error: 
	return false;
}

bool loadMedia()
{
	tree = SDL_LoadBMP("Assets/tree.bmp");
	check(tree != NULL, "Failed to load the asset!");
		
	return true;
error:
	return false;
}

void render()
{
	SDL_BlitSurface(tree, NULL, screenSurface, NULL);
	
	SDL_UpdateWindowSurface(window);
}



int main(int argc, char* argv[])
{

	bool r = init();
	check(r != false, "Failed in Initialization!");
	
	r = loadMedia();
	check(r != false, "Failed in Loading!");
	
	
	render();
	
	SDL_Event e;
	bool quit = false;
	
	while(quit == false){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
	}

error:
	SDL_FreeSurface(tree);
	tree = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	
	SDL_Quit();
}

	
