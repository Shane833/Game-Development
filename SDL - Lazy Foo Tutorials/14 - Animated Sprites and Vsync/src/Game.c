#include <Game.h>
#include <Texture.h>

#define TOTAL_FRAMES 6

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool quit = false;
	
// Texture Variable
Texture sprite_sheet;
SDL_Rect sheet[TOTAL_FRAMES];
int current_frame = 0;

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
		
		current_frame++;
		if(current_frame / TOTAL_FRAMES >= TOTAL_FRAMES){
			current_frame = 0;
		}
		
	}
	
	close();
	
	return 0;
error:
	return 1;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s\n", SDL_GetError());
	
	window = SDL_CreateWindow("Color Keying",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s\n", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s\n", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_Image! IMG_Error: %s\n", IMG_GetError());
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	bool r = Texture_loadFromFile(renderer, &sprite_sheet, "Assets/walk.png");
	check(r == true, "Failed to load the front texture");
	
	
	sheet[0].x = 0;
	sheet[0].y = 0;
	sheet[0].w = 48;
	sheet[0].h = 48;
	
	sheet[1].x = 48;
	sheet[1].y = 0;
	sheet[1].w = 48;
	sheet[1].h = 48;
	
	sheet[2].x = 96;
	sheet[2].y = 0;
	sheet[2].w = 48;
	sheet[2].h = 48;
	
	sheet[3].x = 144;
	sheet[3].y = 0;
	sheet[3].w = 48;
	sheet[3].h = 48;
	
	sheet[4].x = 192;
	sheet[4].y = 0;
	sheet[4].w = 48;
	sheet[4].h = 48;
	
	sheet[5].x = 240;
	sheet[5].y = 0;
	sheet[5].w = 48;
	sheet[5].h = 48;
	
	
	/*
   //Set sprite clips
	sheet[ 0 ].x =   0;
	sheet[ 0 ].y =   0;
	sheet[ 0 ].w =  64;
	sheet[ 0 ].h = 205;

	sheet[ 1 ].x =  64;
	sheet[ 1 ].y =   0;
	sheet[ 1 ].w =  64;
	sheet[ 1 ].h = 205;

	sheet[ 2 ].x = 128;
	sheet[ 2 ].y =   0;
	sheet[ 2 ].w =  64;
	sheet[ 2 ].h = 205;

	sheet[ 3 ].x = 192;
	sheet[ 3 ].y =   0;
	sheet[ 3 ].w =  64;
	sheet[ 3 ].h = 205;
	*/
	
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

void render()
{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); // Clears the current frame
			
		SDL_Rect* current_clip = &sheet[current_frame / TOTAL_FRAMES];	
		Texture_render(renderer, &sprite_sheet, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, current_clip);
		
		SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	Texture_destroy(&sprite_sheet);
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	IMG_Quit();
	SDL_Quit();
}

