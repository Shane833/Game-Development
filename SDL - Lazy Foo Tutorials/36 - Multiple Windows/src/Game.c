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
	
	for(int i = 0;i < TOTAL_WINDOWS;i++){
		gWindows[i] = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
		check(gWindows[i] != NULL, "ERROR : Failed to create window %d",i);
	}
	
	renderer = Window_createRenderer(window);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
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
		
		Window_handleEvents(window, renderer, &e);
	}	
}

void update()
{
	
}

void render()
{
	// Render only when the window is not minimized
	if(!window->minimized){
		// Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); // Clears the current frame
		
		
		SDL_RenderPresent(renderer); // Display the frame to the screen
	}
	
}

void close()
{
	// Close our window and the renderer
	Window_destroy(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
