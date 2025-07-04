#include <Game.h>
#include <DataStream.h>


// Usual Global Variables
Texture * streaming_texture = NULL;
DataStream * stream = NULL;

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
	// Close the texture
	Texture_destroy(streaming_texture);
	streaming_texture = NULL;

	// Close the stream
	DataStream_destroy(stream);
	stream = NULL;

	// Close all of our windows
	Window_destroy(window);
	window = NULL;
	
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
	window = Window_create("Texture Streaming", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window!");

	// Create a Texture
	streaming_texture = Texture_create();
	check(streaming_texture != NULL, "ERROR : Failed to create Texture!");

	// Create the stream
	stream = DataStream_create();
	check(stream != NULL, "ERROR : Failed to create the stream!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	// Create a blank texture
	bool r = Texture_createBlank(streaming_texture, window, 64, 205);
	check(r != false, "ERROR : Failed to create blank texture!");

	r = DataStream_loadMedia(stream);
	check(r != false, "ERROR : Failed to load stream media!");

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
		Window_handleEvents(window, &e);
	
	}	
}

void update()
{

}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window * window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// Copy frame from buffer
		Texture_lockTexture(streaming_texture);
		Texture_copyRawPixels32(streaming_texture, DataStream_getBuffer(stream));
		Texture_unlockTexture(streaming_texture);

		// Render the texture
		Texture_render(streaming_texture, window, (SCREEN_WIDTH - Texture_getWidth(streaming_texture)) / 2,(SCREEN_HEIGHT - Texture_getHeight(streaming_texture)) / 2, NULL);

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

