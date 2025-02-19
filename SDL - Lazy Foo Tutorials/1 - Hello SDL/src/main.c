// Using SDL and Standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h> // provides a new data type 'bool'
#include <dbg.h>

// Screen Dimensions constant
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc,char* argv[])
{
	// The window we'll be rendering to
	SDL_Window* window = NULL;
	
	// The Surface contained by the window
	SDL_Surface* screenSurface = NULL;
	// SDL_Surface is a simple 2D image, which can be loaded from
	// a file or can be the image inside the window
	
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// You cannot directly use the SDL functions directly you would
		// need to initialize them first and since we are only concerend
		// with the video subsytem, we initialize it with the SDL_INIT_VIDEO flag
		printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError()); // This function returns the error for any SDL function
	}
	else
	{
		// create window
		window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		// The first argument is the title of the window
		// The next two arguments defines where on the screen will the window be shown on the screen
		// Since we don't know where to display it we will simply put SDL_WINDOWPOS_UNDEFINED flag
		// The next two arguments determines the width and height of the window
		// The last argument is a flag to let the function we want to show the window on the screen
		if(window == NULL)
		{
			// If window is not created then the function will return NULL and we can 
			// obtain the subsequent cause using SDL_GetError()
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			// Get the window surface
			screenSurface = SDL_GetWindowSurface(window);
			
			// Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			
			// Update the surface
			// This step is very important that once you've created something
			// It won't directly show up on the screen, you must manually update the screen
			SDL_UpdateWindowSurface(window);
			
			// Hack the window to stay up
			SDL_Event e;
			bool quit = false;
			
			while(quit == false)
			{
				while(SDL_PollEvent(&e))
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
			}
		}
		
	}
	
	// Destroy the window
	// This function frees up the memory
	// This will also take care of the SDL_Surface too
	SDL_DestroyWindow(window);
	
	// Quit SDL subsystems
	// This function will close all the subsystems
	SDL_Quit();
	
	return 0;
	
}