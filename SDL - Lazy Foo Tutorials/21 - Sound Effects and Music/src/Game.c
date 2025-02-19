#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <UI.h>


// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture texture; // to hold our single texture

// In order to play music (longer sound) we use Mix_Music data type
Mix_Music* music = NULL;
// For playing shorter sounds we use Mix_Chunk data types
Mix_Chunk* scratch = NULL;
Mix_Chunk* high = NULL;
Mix_Chunk* medium = NULL;
Mix_Chunk* low = NULL;

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	while(!quit)
	{	
		handleEvents();
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
	// 1. frequency (44100 hz its the stadard) 2. Audio format (16bit, 24bit, 32bit) 3. Channels (2 for stereo) 
	// 4. Chunk size or samples per frame (lower the no, lower the latency),2048 is a good default but you can also try 1024, 4096 etc
	
	window = SDL_CreateWindow("Mouse Events and Buttons",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	// First load the texture
	bool r = Texture_loadFromFile(renderer, &texture, "Assets/prompt.png");
	check(r != false, "Failed to load the button texture");
	
	// load music with Mix_LoadMUS
	music = Mix_LoadMUS("Assets/beat.wav");
	check(music != NULL, "Failed to load the music! Mix_Error: %s", Mix_GetError());
	
	// load effects with Mix_LoadWAV
	scratch = Mix_LoadWAV("Assets/scratch.wav");
	check(scratch != NULL, "Failed to load the sound effect! Mix_Error: %s", Mix_GetError());	
	
	high = Mix_LoadWAV("Assets/high.wav");
	check(high != NULL, "Failed to load the sound effect! Mix_Error: %s", Mix_GetError());
	
	medium = Mix_LoadWAV("Assets/medium.wav");
	check(medium != NULL, "Failed to load the sound effect! Mix_Error: %s", Mix_GetError());
	
	low = Mix_LoadWAV("Assets/low.wav");
	check(low != NULL, "Failed to load the sound effect! Mix_Error: %s", Mix_GetError());
	
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
		else if (e.type == SDL_KEYDOWN){
			switch(e.key.keysym.sym){
				
				case SDLK_1:
					Mix_PlayChannel(-1, high, 0); // To play the sound we uses Mix_PlayChannel(channel, sound chunk, no. of times to be repeated)
					break; // since we don't care which channel it comes out of we use -1 to play through the closest channel, and 0 since we don't want to repeat the sound
				
				case SDLK_2:
					Mix_PlayChannel(-1, medium, 0);
					break;
				
				case SDLK_3:
					Mix_PlayChannel(-1, low, 0);
					break;
				
				case SDLK_4:
					Mix_PlayChannel(-1, scratch, 0);
					break;
				
				case SDLK_9:
					// if there is no music playing then play the music
					if(Mix_PlayingMusic() == 0){
						Mix_PlayMusic(music, -1); // the second argument defines how many times we want to repeat the sound
					} 							  // here -1 signfies that we want to repeat it until it is manually stopped
					// and if music is being played then
					else{
						// if music is paused
						if(Mix_PausedMusic() == 1){
							// then resume the music
							Mix_ResumeMusic();
						}
						// If the music is playing
						else{
							// pause the music
							Mix_PauseMusic();
						}
					}
					break;
				
				case SDLK_0:
					// stop the music
					Mix_HaltMusic();
					break;
			}
		}
	}	
	
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Render the texture
	Texture_render(renderer, &texture, 0, 0, NULL);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	// To close our sound effect we use Mix_FreeChunk
	Mix_FreeChunk(scratch);
	Mix_FreeChunk(high);
	Mix_FreeChunk(medium);
	Mix_FreeChunk(low);
	
	scratch = NULL;
	high = NULL;
	medium = NULL;
	low = NULL;
	
	// To close our music types we use Mix_FreeMusic
	Mix_FreeMusic(music);
	music = NULL;
	
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

