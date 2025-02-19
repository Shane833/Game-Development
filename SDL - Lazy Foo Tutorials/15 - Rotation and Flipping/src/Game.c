#include <Game.h>
#include <Texture.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool quit = false;
	
// Texture Variable
Texture arrow;
double degrees = 0;
SDL_RendererFlip flip_type = SDL_FLIP_NONE;

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
	bool r = Texture_loadFromFile(renderer, &arrow, "Assets/arrow.png");
	check(r == true, "Failed to load the front texture");
	
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
		else if(e.type == SDL_KEYDOWN){
			switch(e.key.keysym.sym){
				case SDLK_a:
					degrees -= 60;
					break;
					
				case SDLK_d:
					degrees += 60;
					break;
					
				case SDLK_q:
					flip_type = SDL_FLIP_HORIZONTAL;
					break;
				
				case SDLK_w:
					flip_type = SDL_FLIP_NONE;
					break;
				
				case SDLK_e:
					flip_type = SDL_FLIP_VERTICAL;
					break;
			}
		}
		
	}
}

void render()
{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer); // Clears the current frame
				
		Texture_renderEx(renderer, &arrow, (SCREEN_WIDTH - Texture_getWidth(&arrow)) / 2, (SCREEN_HEIGHT - Texture_getHeight(&arrow)) / 2, NULL, degrees, NULL, flip_type);
		
		SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	Texture_destroy(&arrow);
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	IMG_Quit();
	SDL_Quit();
}

