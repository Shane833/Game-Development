#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

typedef struct{
    /* Window Members */
	SDL_Window * window; 
	SDL_Renderer * renderer; 
	int windowID; 
    /* Window Dimensions */
	int width;
	int height;
    /* Window Focus */
	bool mouse_focus;
	bool keyboard_focus;
	bool fullscreen;
	bool minimized;
	bool shown;
}Window;

// Window Functions
Window * Window_create(int width, int height, Uint32 flags);
void Window_handleEvents(Window * window, SDL_Event * e);
void Window_focus(Window * window);
extern void Window_render(Window * window);
void Window_destroy(Window * window);


#endif