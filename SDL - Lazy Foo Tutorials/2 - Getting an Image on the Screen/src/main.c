// Now we will divide the whole process into functions
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

// Global Variables
// Window we'll be rendering to
SDL_Window* window = NULL;
// Window Surface we'll be drawing to
SDL_Surface* screenSurface = NULL;
// The image we will load and dispaly on the screen
SDL_Surface* tree = NULL;

// SDL_Surface is rendered in the software i.e. it is being rendered by the CPU
// We are currently using to keep things simple, it just contains the basic pixel data
// and some additional data to help it render on the screen

// Function prototyping
// Starts up SDL and initalizes window
bool init(); 
// loads the media
bool loadMedia();
// Frees up the resources and closes SDL
void close();
 
int main(int argc, char* argv[]){

	// Calling the function on by one
	bool r = init();
	check(r == true, "Something went wrong!!!");
	
	r = loadMedia();
	check(r == true, "Something went wrong!!!");
	
	// Blitting means taking a copy on one thing and stamping
	// it on the the other 
	// src(1st) and dest(3rd) don't worry about the 2nd and 4th arg now
	SDL_BlitSurface(tree, NULL, screenSurface, NULL); // Applying the image
	
	// Updating the window surface
	SDL_UpdateWindowSurface(window);
	
	/*
	After drawing everything on the screen that we want to show for this 
	frame we have to update the screen using SDL_UpdateWindowSurface. 
	See when you draw to the screen, you are not typically drawing to the image on the screen you see. 
	By default, most rendering systems out there are double buffered. These two buffers are the front 
	and back buffer.
	When you make draw calls like SDL_BlitSurface, you render to the back buffer. 
	What you see on the screen is the front buffer. The reason we do this is because most frames require 
	drawing multiple objects to the screen. If we only had a front buffer, we would be able to see the 
	frame as things are being drawn to it which means we would see unfinished frames. 
	So what we do is draw everything to the back buffer first and once we're done 
	we swap the back and front buffer so now the user can see the finished frame.
	*/
	
	// Keeping the window alive
	SDL_Event e;
	bool quit = false;
	
	while(quit == false){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
	}
	
	// Freeing the resources
	close();
	
return 0;
error:
	return 1;
}

// Function definitions
bool init(){
	// Customizing function according to me 
	check(SDL_Init(SDL_INIT_VIDEO) >= 0,"Failed to initialize SDL! SDL_Error : %s\n", SDL_GetError());
	// SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Getting an Image on the Screen",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create the window! SDL_Error: %s", SDL_GetError());
	
	screenSurface = SDL_GetWindowSurface(window);
	// We call this function to retreive the pointer to the image inside the window
	// So that we can load up a image and show it up on the screen
	
	return true;
	error:
		return false;
}

bool loadMedia(){
	// Loads up the image 
	tree = SDL_LoadBMP("Assets/tree.bmp");
	check(tree != NULL,"Falied to load tree.bmp! SDL_Error: %s\n", SDL_GetError());
	
	return true;
	error:
		return false;
}

void close(){
	// Deallocate the surface
	SDL_FreeSurface(tree);
	tree = NULL;
	
	// Destroy the window
	SDL_DestroyWindow(window);
	window = NULL;
	
	// Quit SDL Subsystems
	SDL_Quit();
}
	
	
