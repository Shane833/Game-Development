#include <Game.h>


// Usual Global Variables
Window* gWindow; 

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TTF_Font* font = NULL;
bool quit = false;

Texture stickman;

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
	
	return 0;
error:
	return 1;
}

int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	// Close all of our windows
	Window_destroy(gWindow);
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	gWindow = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(gWindow != NULL, "ERROR : Failed to create window");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	bool r = Texture_loadPixelsFromFile(gWindow, &stickman, "Assets/foo.png");
	check(r == true, "ERROR : Failed to load the pixels from the file!");
	
	// Get the pixel data
	Uint32* pixels = Texture_getPixels32(&stickman);
	int pixel_count = Texture_getPitch32(&stickman) * Texture_getHeight(&stickman);
	
	// Map colors, for manually color keying
	Uint32 color_key = Texture_mapRGBA(&stickman, 255, 0, 255, 255);
	Uint32 transparent = Texture_mapRGBA(&stickman, 255, 255, 255, 0);
	
	// Color key pixels
	for(int i = 0;i < pixel_count;i++){
		if(pixels[i] == color_key){
			pixels[i] = transparent;
		}	
	}
	
	// Create the texture from manually color keyed pixels
	r = Texture_loadFromPixels(gWindow, &stickman);
	check(r == true, "ERROR : Failed to create texture from pixels!");
	
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
	
	}	
}

void update()
{
}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window* window)
{
	// Again we only want to draw to a window if its not minimized
	if(!gWindow->minimized){
		// clear screen
		SDL_SetRenderDrawColor(gWindow->renderer, 255,255,255,255);
		SDL_RenderClear(gWindow->renderer);
	
		Texture_render(gWindow, &stickman, (SCREEN_WIDTH / 2) - Texture_getWidth(&stickman), (SCREEN_HEIGHT / 2) - Texture_getHeight(&stickman), NULL);
		// update screen
		SDL_RenderPresent(gWindow->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(gWindow);
}

void close()
{
	
}
