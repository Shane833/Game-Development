#include <Game.h>

// Usual Global Variables
BitmapFont * bfont = NULL;

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
	// Close all of our windows
	Window_destroy(window);
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
	window = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window");

	// Create the font
	bfont = BitmapFont_create();
	check(bfont != NULL, "ERROR : Failed to initialize Bitmap Font!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	// Build the font
	bool r = BitmapFont_buildFont(bfont, window, "Assets/lazyfont.png");
	check(r != false, "ERROR : Failed to build Bitmap Font!");

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

}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window * window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		BitmapFont_render(bfont, window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "Hello!");

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

