#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <UI.h>
#include <Timer.h>
#include <Dot.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture dot_texture;
Dot* dot = NULL;
Dot* otherdot = NULL;
SDL_Rect wall;

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
	
	window = SDL_CreateWindow("Advanced Timers",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	bool r = Texture_loadFromFile(renderer, &dot_texture, "Assets/dot.bmp");
	check(r != false, "Failed to load the texture!");
	
	dot = Dot_create(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3);
	check(dot != NULL, "Failed to create the dot!");
	
	otherdot = Dot_create(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
	check(otherdot != NULL, "Failed to create the otherdot!");

	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400;
	
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
		
		// Handle the dot events
		Dot_handleEvents(dot, &e);
	}	
}

void update()
{
	Dot_move(dot, &wall, Dot_getCollider(otherdot), SCREEN_WIDTH, SCREEN_HEIGHT);
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// render the wall
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &wall);
	
	// Render the dot
	Dot_render(renderer, &dot_texture, otherdot);
	Dot_render(renderer, &dot_texture, dot);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	free(dot);
	dot = NULL;
	
	Texture_destroy(&dot_texture);
	
	// Close our window and the renderer
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
