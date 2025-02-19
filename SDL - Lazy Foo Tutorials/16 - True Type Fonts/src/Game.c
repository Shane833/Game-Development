#include <Game.h>
#include <Texture.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;
	
// Texture Variable
Texture text_texture;

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
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); // returns 0 on success and -1 on failure
	
	window = SDL_CreateWindow("Color Keying",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	// First we will load the global font using TTF_OpenFont(path, size) function
	font = TTF_OpenFont("Assets/lazy.ttf", 28);
	check(font != NULL, "Failed to load the font! TTF_Error : %s", TTF_GetError());
	
	// Set up the color you want your text to be rendered in
	SDL_Color text_color = {0, 0, 0}; // RGB
	
	// create the texture
	bool r = Texture_loadFromRenderedText(renderer, &text_texture, font, "The quick brown fox jumps over the lazy dog", text_color);
	check(r != false, "Failed to load the rendered text!");
	
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
				
		Texture_render(renderer, &text_texture, (SCREEN_WIDTH - Texture_getWidth(&text_texture)) / 2, (SCREEN_HEIGHT - Texture_getHeight(&text_texture)) / 2,NULL);
		
		SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	Texture_destroy(&text_texture);
	
	// Close the global font
	TTF_CloseFont(font);
	font = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

