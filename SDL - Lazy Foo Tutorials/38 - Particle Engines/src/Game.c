#include <Game.h>


// Usual Global Variables
Window* gWindow; 
Dot* dot;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture dot_texture;
Texture texture_array[4];

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
	
	// Here we initialize the window
	gWindow = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
	check(gWindow != NULL, "ERROR : Failed to create window");
	
	dot = Dot_create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	check(dot != NULL, "ERROR : Failed to create the dot!");

	return true;
error:
	return false;
}

bool loadMedia()
{
	bool r = Texture_loadFromFile(gWindow->renderer, &dot_texture, "Assets/dot.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
	r = Texture_loadFromFile(gWindow->renderer, &texture_array[RED], "Assets/red.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
	r = Texture_loadFromFile(gWindow->renderer, &texture_array[GREEN], "Assets/green.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
	r = Texture_loadFromFile(gWindow->renderer, &texture_array[BLUE], "Assets/BLUE.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
	r = Texture_loadFromFile(gWindow->renderer, &texture_array[SHIMMER], "Assets/shimmer.bmp");
	check(r != false, "ERROR : Failed to load the texture!");
	
	// Then we make the textures a little translucent
	Texture_setAlpha(&dot_texture, 192);
	Texture_setAlpha(&texture_array[RED], 192);
	Texture_setAlpha(&texture_array[BLUE], 192);
	Texture_setAlpha(&texture_array[GREEN], 192);
	Texture_setAlpha(&texture_array[SHIMMER], 192);
	
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
	Dot_move(dot, SCREEN_WIDTH, SCREEN_HEIGHT);
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
		Dot_render(window->renderer, &dot_texture, texture_array, dot);
		
		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(gWindow);
}

void close()
{
	// Close all of our windows
	Window_destroy(gWindow);	
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
