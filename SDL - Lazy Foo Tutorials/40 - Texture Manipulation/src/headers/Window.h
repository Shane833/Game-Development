#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

typedef struct{
	SDL_Window* window; // window
	SDL_Renderer* renderer; // renderer
	int windowID; // window ID
	// Window Dimensions
	int width;
	int height;
	// Window Focus
	bool mouse_focus;
	bool keyboard_focus;
	bool fullscreen;
	bool minimized;
	bool shown;
}Window;

// Window Functions
Window* Window_create(int width, int height, Uint32 flags);
void Window_handleEvents(Window* window, SDL_Event* e);
void Window_focus(Window* window);
extern void Window_render(Window* window);
void Window_destroy(Window* window);


#endif