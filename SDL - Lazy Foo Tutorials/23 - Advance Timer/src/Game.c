#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <UI.h>
#include <Timer.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture time_texture; // to hold our single texture
Texture start_prompt_texture; // to display our prompt for the user
Texture pause_prompt_texture; // to display our prompt for the user
Timer *timer; // our timer
char time[32]; // string to store our time current time
SDL_Color text_color = {0, 0, 0, 255}; // setting the white color

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
	// load the font
	font = TTF_OpenFont("Assets/lazy.ttf",50);
	check(font != NULL, "Unable to load the font! TTF_Error: %s",TTF_GetError());
	
	// Creating our prompt
	bool r = Texture_loadFromRenderedText(renderer, &start_prompt_texture, font, "Press s to start/stop", text_color);
	check(r != false,"Failed to generate texture from text!");
	
	r = Texture_loadFromRenderedText(renderer, &pause_prompt_texture, font, "Press p to pause/resume", text_color);
	check(r != false,"Failed to generate texture from text!");
	
	timer = Timer_create(); // initialize our timer
	check(timer != NULL, "Unable to create the timer! SDL_Error: %s", SDL_GetError());
	
	
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
		// Handle the key presses
		else if(e.type == SDL_KEYDOWN){
			
			switch(e.key.keysym.sym){
				case SDLK_s: // start stop the timer
					if(Timer_isStarted(timer)){
						Timer_stop(timer);
					}
					else{
						Timer_start(timer);
					}
					break;
				
				case SDLK_p: // pause/resume timer
					if(Timer_isPaused(timer)){
						Timer_resume(timer);
					}
					else{
						Timer_pause(timer);
					}
					break;		
			}
		}
	}	
}

void update()
{
	Uint32 temp_time = Timer_getTicks(timer) / 1000.f; // since we want the in seconds and by default it is milliseconds(1/1000 of a second)
	// and now to convert from integer to string using sprintf function
	sprintf(time, "%ld", temp_time);
	// now we generate the texture from this text and our font
	bool r = Texture_loadFromRenderedText(renderer, &time_texture, font, time, text_color);
	check(r != false, "Unable to create a texture from text!");
	
error: // fallthrough
	return;
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// render the updated texture
	Texture_render(renderer, &start_prompt_texture, 0, 0, NULL);
	Texture_render(renderer, &pause_prompt_texture, 0, 100, NULL);
	Texture_render(renderer, &time_texture, (SCREEN_WIDTH - Texture_getWidth(&time_texture)) / 2 , (SCREEN_HEIGHT - Texture_getHeight(&time_texture)) / 2, NULL);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	Texture_destroy(&time_texture);
	Texture_destroy(&start_prompt_texture);
	Texture_destroy(&pause_prompt_texture);
	
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

