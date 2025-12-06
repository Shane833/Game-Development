#include <Game.h>
#include <Timer.h>
#include <Sound.h>
#include <bstrlib.h>
#include <Dot.h>

// Usual Global Variables
SDL_Color text_color = {0,0,0,255};
Timer * fps_timer = NULL; // timer to keep track of ms passed
Dot* dot = NULL; // our dot
SDL_Rect dotCamera = {.x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT};


int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	// Destroy the dot
	Dot_destroy(dot);
	dot = NULL;

	// Close the timer
	Timer_destroy(fps_timer);
	fps_timer = NULL;

	// Close all of our windows
	Window_destroy(window);
	window = NULL;

	// Close the font
	TTF_CloseFont(font);
	font = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	// Start the timer before entering the loop
	Timer_start(fps_timer);

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


// Function definitions
bool init()
{
	// Now we initialize the audio subsystem too
	check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	window = Window_create("Frame Independent Movement!", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window!");

	// Create our timer
	fps_timer = Timer_create();
	check(fps_timer != NULL, "ERROR : Failed to create the timer!");

	// Creating the dot
	dot = Dot_create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	check(dot != NULL, "ERROR: Failed to create the dot!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	bool r = false;

	// load the font from the file
	font = TTF_OpenFont("Assets/lazy.ttf", 28);
	check(font != NULL, "ERROR : Failed to load the font!, TTF Error : %s", TTF_GetError());

	// loading the dot texture
	r = Texture_loadFromFile(&dotTexture, window, "Assets/dot.bmp");
	check(r != false, "ERROR: Failed to load the dot texture, SDL Error: %s", SDL_GetError());
		
	return r;
error:
	return false;
}

void handleEvents()
{	
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}

		// Handle all the window events
		Window_handleEvents(window, &e);

		// Handling the dot events
		Dot_handleEvents(dot, &e);
	
	}

}

void update()
{
	// Updating the dot position
	Dot_move(dot, SCREEN_WIDTH, SCREEN_HEIGHT);
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

		Dot_render(dot, window, &dotCamera);
		
		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

