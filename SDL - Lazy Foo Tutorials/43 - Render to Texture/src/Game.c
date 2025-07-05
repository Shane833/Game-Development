#include <Game.h>

// Usual Global Variables
Texture * target_texture = NULL;
// Rotation variable
double angle = 0.0;
SDL_Point screen_center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	while(!quit)
	{	
		handleEvents();
		update();
		render();	
	}
	
	return 0;
error:
	return 1;
}

int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	// Close the texture
	Texture_destroy(target_texture);
	target_texture = NULL;

	// Close all of our windows
	Window_destroy(window);
	window = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	window = Window_create("Texture Streaming", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window!");

	// Create a Texture
	target_texture = Texture_create();
	check(target_texture != NULL, "ERROR : Failed to create Texture!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	// Create a blank texture
	bool r = Texture_createBlank(target_texture, window, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_TEXTUREACCESS_TARGET);
	check(r != false, "ERROR : Failed to create blank texture!");

	return true;
error:
	return false;
}

void handleEvents()
{
	SDL_Event e; // Queue to store the events
	
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}
		
		// Handle all the window events
		Window_handleEvents(window, &e);
	
	}	
}

void update()
{
	// rotate
	angle += 2.0;

	if(angle > 360.0){
		angle -= 360; // just get back 
	}

}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window * window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		
		// Set the texture as render target
		Texture_setAsRenderTarget(target_texture, window);

		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// Render red fille quad
		SDL_Rect fill_rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_SetRenderDrawColor(window->renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(window->renderer, &fill_rect);

		// Render green outlined quad
		SDL_Rect outline_rect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
		SDL_SetRenderDrawColor(window->renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(window->renderer, &outline_rect);

		// Draw blue horizontal line
		SDL_SetRenderDrawColor(window->renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(window->renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

		// Draw vertical line of yellow dots
		SDL_SetRenderDrawColor(window->renderer, 255, 255, 0, 255);
		for(int i = 0;i < SCREEN_HEIGHT; i++){
			SDL_RenderDrawPoint(window->renderer, SCREEN_WIDTH / 2, i);
		}

		// Uptill this point all the data has been rendered to the texture
		// Now we reset the render target so that we can render our texture
		// to the screen with the help of the renderer

		// Reset the render target
		SDL_SetRenderTarget(window->renderer, NULL); // resets the render target to default i.e. the screen

		// Show the rendered texture
		Texture_renderEx(target_texture, window, 0, 0, NULL, angle, &screen_center, SDL_FLIP_NONE);

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

