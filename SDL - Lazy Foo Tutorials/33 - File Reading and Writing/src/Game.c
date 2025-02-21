#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <bstrlib.h>

#define TOTAL_DATA 10

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture prompt_text_texture;
Texture gDataTextures[TOTAL_DATA];
// Text Color
SDL_Color text_color = {0,0,0,255};
SDL_Color highlight_color = {255,0,0,255};
// Input text string
bstring input_text;
// Data Points
Sint32 gData[TOTAL_DATA];
// String for converting between integer and string
char str_data[32];
// index for pointing to the current data
int current_data = 0;

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
	
	// Initializing the string
	input_text = bfromcstr("Some Text");
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	font = TTF_OpenFont("Assets/Lazy.ttf", 28);
	check(font != NULL, "Failed to load the Font!, TTF_ERROR : %s", TTF_GetError());
	
	// Try and open the file
	SDL_RWops* file = SDL_RWFromFile("Assets/nums.bin", "r+b");
	
	// File does not exist
	if (file == NULL){
		log_warn("Warning : Unable to open file! SDL Error : %s", SDL_GetError());
		
		// Create a new file for writing
		file = SDL_RWFromFile("Assets/nums.bin", "w+b");
		check(file != NULL, "ERROR : Unable to create file! SDL Error : %s", SDL_GetError());
		
		// Initialize the data
		for(int i = 0;i < TOTAL_DATA;i++){
			gData[i] = 0;
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}
	} else {
		// Load Data
		log_info("[INFO] : Reading file...");
		for(int i = 0;i < TOTAL_DATA;i++){
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
		}
	}
	
	// Close File handler
	SDL_RWclose(file);
	
	// Initialize the data textures
	sprintf(str_data,"%d", gData[0]);
	input_text = bfromcstr(str_data);
	
	Texture_loadFromRenderedText(renderer, &gDataTextures[0], font, bdata(input_text), highlight_color);
	for(int i = 1;i < TOTAL_DATA;i++){
		sprintf(str_data,"%d", gData[i]);
		input_text = bfromcstr(str_data);
		Texture_loadFromRenderedText(renderer, &gDataTextures[i], font, bdata(input_text), text_color);
	}

	Texture_loadFromRenderedText(renderer, &prompt_text_texture, font, "Enter Data:", text_color);
	
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
		else if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					// Previous data entry
					case SDLK_UP:
						// Render previous entry input in regular color
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), text_color); 
						
						--current_data;
						
						// Check out of bounds condition
						if(current_data < 0){
							current_data = TOTAL_DATA - 1;
						}
						
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						// Render the current input point
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), highlight_color); 
						break;
					
					case SDLK_DOWN:
						// Render previous entry input in regular color
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), text_color); 
						
						++current_data;
						
						// Check out of bounds condition
						if(current_data == TOTAL_DATA){
							current_data = 0;
						}
						
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						// Render the current input point
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), highlight_color); 
						break;	
					
					case SDLK_LEFT:
						--gData[current_data];
						
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), highlight_color);
						break;
					
					case SDLK_RIGHT:
						++gData[current_data];
						
						sprintf(str_data,"%d", gData[current_data]);
						input_text = bfromcstr(str_data);
						
						Texture_loadFromRenderedText(renderer, &gDataTextures[current_data], font, bdata(input_text), highlight_color);
						break;
				}
		}
	}	
}

void update()
{
	
}

void render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Render text textures
	Texture_render(renderer, &prompt_text_texture, (SCREEN_WIDTH - Texture_getWidth(&prompt_text_texture)) / 2, 0, NULL);

	for(int i = 0;i < TOTAL_DATA;i++){
		Texture_render(renderer, &gDataTextures[i], (SCREEN_WIDTH - Texture_getWidth(&gDataTextures[i])) / 2, Texture_getHeight(&prompt_text_texture) + Texture_getHeight(&gDataTextures[i]) * i, NULL);
	}
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	// Open Data for writing
	SDL_RWops* file = SDL_RWFromFile("Assets/nums.bin","w+b");
	if(file){
		// Save data
		for(int i = 0;i < TOTAL_DATA;i++){
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}
		// Close File Handler
		SDL_RWclose(file);
	}
	else{
		log_info("[INFO] : Unable to save file!, SDL ERROR : %s", SDL_GetError());
	}
	
	// destroy the string
	bdestroy(input_text);
	
	
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
