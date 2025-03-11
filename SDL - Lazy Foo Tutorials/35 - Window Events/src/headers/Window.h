#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>
#include <bstrlib.h>

typedef struct{
	// main windows
	SDL_Window* window; 
	// Window Dimensions
	int width;
	int height;
	// Window Focus
	bool mouse_focus;
	bool keyboard_focus;
	bool fullscreen;
	bool minimized;
}Window;

// Window Functions

Window* Window_createWindow(int width, int height);
SDL_Renderer* Window_createRenderer(Window* window);
void Window_handleEvents(Window* window, SDL_Renderer* renderer, SDL_Event* e);
void Window_destroy(Window* window);

#endif