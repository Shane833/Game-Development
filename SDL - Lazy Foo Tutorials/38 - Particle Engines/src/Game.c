#include <Game.h>

#define TOTAL_WINDOWS 3

// Usual Global Variables
Window* gWindows[TOTAL_WINDOWS]; // Array of windows

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture scene_texture;

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
	
	close();
	
	return 0;
error:
	return 1;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	check(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0, "Failed to initialize SDL_mixer! Mix_Error: %s", Mix_GetError());
	
	// Here we initialize the windows
	for(int i = 0;i < TOTAL_WINDOWS;i++){
		gWindows[i] = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
		check(gWindows[i] != NULL, "ERROR : Failed to create window %d",i);
	}
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	
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
		for(int i = 0;i < TOTAL_WINDOWS;i++){
			Window_handleEvents(gWindows[i], &e);
		}
		
		// Pull up window with 1,2 and 3 key respectively
		if(e.type == SDL_KEYDOWN){
			switch(e.key.keysym.sym){
				case SDLK_1:
					Window_focus(gWindows[0]);
					break;
				case SDLK_2:
					Window_focus(gWindows[1]);
					break;
				case SDLK_3:
					Window_focus(gWindows[2]);
					break;
			}
		}
	}	
}

void update()
{
	// Check all windows
	// If all the windows are closed then we simply quit out of the application
	bool all_windows_closed = true;
	for(int i = 0;i < TOTAL_WINDOWS;i++){
		if((gWindows[i])->shown){
			all_windows_closed = false;
			break;
		}
	}
	
	// Application closed all windows
	if(all_windows_closed){
		quit = true;
	}
}

void render()
{
	// Render all the windows
	for(int i = 0;i < TOTAL_WINDOWS;i++){
		Window_render(gWindows[i]);
	}
	
}

void close()
{
	// Close all of our windows
	for(int i = 0;i < TOTAL_WINDOWS;i++){
		Window_destroy(gWindows[i]);	
	}
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
