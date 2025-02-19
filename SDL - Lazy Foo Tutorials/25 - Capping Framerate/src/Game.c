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
const int SCREEN_FPS = 60; // This is our desired FPS that we need
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS; // This defines the amount of times each frame will get (here for 60, its 16.7 ms)
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Timer* fps_timer = NULL; // This tells our current FPS
Timer* cap_timer = NULL; // We will use this timer to cap our framerate
int counted_frames = 0; // Tells the no.of frames that have passed
float fps = 0.0f; // to display our fps

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	// before entering the main loop we start our timer
	Timer_start(fps_timer);
	
	while(!quit)
	{	
		// Inside our loop we start our cap timer
		Timer_start(cap_timer);
		
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
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // we are removing the automatic vsync as we will handle the framerate ourselves
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	return true;
error:
	return false;
}

bool loadMedia()
{	
	// creating our timers
	fps_timer = Timer_create();
	check(fps_timer != NULL, "Failed to create the timer!");
	
	cap_timer = Timer_create();
	check(cap_timer != NULL, "Failed to create the timer!");
	
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
		
	}	
}

void update()
{
	// Calculating the correct fps
	fps = counted_frames / (Timer_getTicks(fps_timer) / 1000.f);
	if(fps > 200000)
		fps = 0;
	
	printf("Avg FPS : %f\n", fps); // displaying our framerate	
	
error: // fallthrough
	return;
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
		
	SDL_RenderPresent(renderer); // Display the frame to the screen
	
	// increment the counted frames
	++counted_frames;
	
	// if frame finished early we want to cap it
	int frame_ticks = Timer_getTicks(cap_timer);
	if(frame_ticks < SCREEN_TICKS_PER_FRAME){
		// Wait the remaining time
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
	}
}

void close()
{	
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

