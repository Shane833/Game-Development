#include <Window.h>

Window* Window_create(int width, int height)
{
	// This time each window have their separate renderers
	Window* temp = malloc(sizeof(Window));
	check(temp != NULL, "ERROR : Failed to create the window!");
	
	// Create the window
	temp->window = SDL_CreateWindow("Window Events", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	check(temp->window != NULL, "ERROR : Failed to create the window!");
	
	// Creating the renderer
	temp->renderer = SDL_CreateRenderer(temp->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(temp->renderer != NULL, "ERROR : Failed to create the window!");
	SDL_SetRenderDrawColor(temp->renderer, 255,255,255,255); // Initialize the render color
	
	temp->width = width;
	temp->height = height;
	
	temp->mouse_focus = true;
	temp->keyboard_focus = true;
	
	temp->fullscreen = false;
	temp->minimized = false;
	
	// Grab the window identifier
	temp->windowID = SDL_GetWindowID(temp->window); // we need this ID for event handling
	
	// Raise the shown flag
	temp->shown = true;
	
	return temp;
	
error:
	Window_destroy(temp);
	return NULL;
}


void Window_handleEvents(Window* window, SDL_Event* e)
{
	// check if window events has occured on the given window
	// All events from all the windows go to the same event queue we need to identify
	// the window based on it window ID
	if (e->type == SDL_WINDOWEVENT && e->window.windowID == window->windowID){
		// Caption update flag
		bool update_caption = false;
		
		switch(e->window.event){
			// Now we don't want to exit out of the program when the user presses the X button
			// So in order to handle that we will simply hide/show the window and check their 
			// corresponding events
			// Window appeared
			case SDL_WINDOWEVENT_SHOWN:
				window->shown = true;
				break;
			
			// Window disappeared
			case SDL_WINDOWEVENT_HIDDEN:
				window->shown = false;
				break;
				
			// Get new dimensions and repaint on size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				window->width = e->window.data1;
				window->height = e->window.data2;
				SDL_RenderPresent(window->renderer);
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
			
			// Hide when closing the window
			case SDL_WINDOWEVENT_CLOSE: // This event gets triggered on clikcing the X button
				SDL_HideWindow(window->window);
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

void Window_focus(Window* window)
{
	// Restore the window if needed
	if(!window->shown){ // If the window is hidden 
		SDL_ShowWindow(window->window); // we show it
	}
	// Move window forward
	SDL_RaiseWindow(window->window); // we bring it above other windows
}

void Window_render(Window* window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void Window_destroy(Window* window)
{
	if(window){
		if(window->window){
			SDL_DestroyWindow(window->window);
			window->window = NULL;	
		}
		if(window->renderer){
			SDL_DestroyRenderer(window->renderer);
			window->renderer = NULL;
		}
		free(window);
	}
}