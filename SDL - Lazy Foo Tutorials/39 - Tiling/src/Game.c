#include <Game.h>


// Usual Global Variables
Window* gWindow; 
Dot* dot;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

TTF_Font* font = NULL;
bool quit = false;

Tile* tiles[10];

SDL_Rect camera = {0,0,640,480};

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia(tiles);
	check(r == true, "Something went wrong");
	
	while(!quit)
	{	
		handleEvents();
		update();
		render();	
	}
	
	close(tiles);
	
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
	
	// Here we initialize the window
	gWindow = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
	check(gWindow != NULL, "ERROR : Failed to create window");
	
	dot = Dot_create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	check(dot != NULL, "ERROR : Failed to create the dot!");

	return true;
error:
	return false;
}

bool loadMedia(Tile* tiles[])
{
	bool r = Texture_loadFromFile(gWindow->renderer, &dotTexture, "Assets/dot.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
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
		Window_handleEvents(gWindow, &e);
		
		// Handle all the dot events
		Dot_handleEvents(dot, &e);
	}	
}

void update()
{
	// Move the dot
	Dot_move(dot, tiles, LEVEL_WIDTH, LEVEL_HEIGHT);
}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window* window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// Then we render the dot
		Dot_render(window->renderer, &camera, &dotTexture, dot);
		
		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(gWindow);
}

void close(Tile* tiles[])
{
	// Destroy the dot
	Dot_destroy(dot);
	
	// Close all of our windows
	Window_destroy(gWindow);	
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
