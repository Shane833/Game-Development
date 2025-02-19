#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <Dot.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture dot_texture;
Texture bg_texture;
Dot* dot = NULL;
SDL_Rect camera = {0,0,0,0};

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
	
	r = Texture_loadFromFile(renderer, &bg_texture, "Assets/bg.png");
	check(r != false, "Failed to load the bg texture!");
	
	dot = Dot_create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	check(dot != NULL, "Failed to create the dot!");
	
	camera.x = 0;
	camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
	
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
	// Move the dot
	Dot_move(dot, LEVEL_WIDTH, LEVEL_HEIGHT);
	
	// Center the camera over the dot
	camera.x = (dot->position.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (dot->position.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;
	
	// Keep the camera in bounds
	if(camera.x < 0){
		camera.x = 0;
	}
	if(camera.y < 0){
		camera.y = 0;
	}
	if(camera.x > LEVEL_WIDTH - camera.w){
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if(camera.y > LEVEL_HEIGHT - camera.h){
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// render the background
	Texture_render(renderer, &bg_texture, 0, 0, &camera);
	
	// render the dot
	Dot_render(renderer, &dot_texture, dot, camera.x, camera.y);
	
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
