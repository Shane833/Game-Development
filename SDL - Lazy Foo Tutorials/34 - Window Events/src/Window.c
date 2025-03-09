#include <Window.h>

Window* Window_createWindow(int width, int height)
{
	Window* temp = malloc(sizeof(Window));
	check(temp != NULL, "ERROR : Failed to create the window!");
	
	// Create the window
	temp->window = SDL_CreateWindow("Window Events", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	check(temp->window != NULL, "ERROR : Failed to create the window!");
	
	temp->width = width;
	temp->height = height;
	
	temp->mouse_focus = true;
	temp->keyboard_focus = true;
	
	temp->fullscreen = false;
	temp->minimized = false;
	
	return temp;
	
error:
	return NULL;
}

SDL_Renderer* Window_createRenderer(Window* window)
{
	check(window != NULL, "ERROR : Invalid Window! Failed to create renderer!");
	
	return SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

error:
	return NULL;
}

void Window_handleEvents(Window* window, SDL_Renderer* renderer, SDL_Event* e)
{
	// check if window events has occured
	if (e->type == SDL_WINDOWEVENT){
		// Caption update flag
		bool update_caption = false;
		
		switch(e->window.event){
			// Get new dimensions and repaint on size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window->width = e->window.data1;
				window->height = e->window.data2;
				SDL_RenderPresent(renderer);
				break;
			
			// Repaint on exposure
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(renderer);
				break;
			
			// Mouse entered window
			case SDL_WINDOWEVENT_ENTER:
				window->mouse_focus = true;
				update_caption = true;
				break;
			
			// Mouse left window
			case SDL_WINDOWEVENT_LEAVE:
				window->mouse_focus = false;
				update_caption = true;
				break;
			
			// Window has keyboard focus
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				window->keyboard_focus = true;
				update_caption = true;
				break;
			
			// Window lost keyboard focus
			case SDL_WINDOWEVENT_FOCUS_LOST:
				window->keyboard_focus = false;
				update_caption = true;
				break;
			
			// Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
				window->minimized = true;
				break;
			
			// Window maximized
			case SDL_WINDOWEVENT_MAXIMIZED:
				window->minimized = false;
				break;
			
			// Window restored
			case SDL_WINDOWEVENT_RESTORED:
				window->minimized = false;
				break;
		}
		
		// Update window caption with new data
		if(update_caption){
			bstring caption = bfromcstr("SDL Tutorial - Mouse Focus: ");
			bcatcstr(caption, window->mouse_focus ? "On Keyboard Focus: " : "Off Keyboard Focus: ");
			bcatcstr(caption, window->keyboard_focus ? "On" : "Off");
			
			SDL_SetWindowTitle(window->window, bdata(caption));	
		}
	}
	else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN){
		if(window->fullscreen){
			SDL_SetWindowFullscreen(window->window, 0);
			window->fullscreen = false;
		}
		else{
			SDL_SetWindowFullscreen(window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			window->fullscreen = true;
			window->minimized = false;
		}
	}
}

void Window_destroy(Window* window)
{
	if(window){
		if(window->window){
			SDL_DestroyWindow(window->window);
			window->window = NULL;	
		}
		free(window);
	}
}