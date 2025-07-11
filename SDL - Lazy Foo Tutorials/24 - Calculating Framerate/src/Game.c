#include <Game.h>
#include <Timer.h>
#include <bstrlib.h>

// Usual Global Variables
Texture * fps_text_texture = NULL; // texture to be displayed on screen
SDL_Color text_color = {0,0,0,255}; // setting the text color to be black
Timer * fps_timer = NULL; // timer to keep track of ms passed
bstring time_text = NULL; // used for string as a time
int counted_frames = 0; // counts the no. of frames per second


int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	// Destroy the string
	bdestroy(time_text);
	time_text = NULL;

	// Close the timer
	Timer_destroy(fps_timer);
	fps_timer = NULL;

	// Close the texture
	Texture_destroy(fps_text_texture);
	fps_text_texture = NULL;

	// Close all of our windows
	Window_destroy(window);
	window = NULL;

	// Close the font
	TTF_CloseFont(font);
	font = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	// Start the timer before entering the loop
	Timer_start(fps_timer);

	while(!quit)
	{	
		handleEvents();
		update();
		render();	
		// Increment the no. of frames passed each time we complete the loop
		counted_frames++;
	}
	
	return 0;
error:
	return 1;
}


// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	window = Window_create("Calculating Framerate", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window!");

	// Create a Texture
	fps_text_texture = Texture_create();
	check(fps_text_texture != NULL, "ERROR : Failed to create Texture!");

	// Create our timer
	fps_timer = Timer_create();
	check(fps_timer != NULL, "ERROR : Failed to create the timer!");

	// Create our bstring
	time_text = bfromcstr("Average FPS : ");
	check(time_text != NULL, "ERROR : Failed to create the bstring!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	// load the font from the file
	font = TTF_OpenFont("Assets/lazy.ttf", 28);
	check(font != NULL, "ERROR : Failed to load the font!, TTF Error : %s", TTF_GetError());
	
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
	// Calculate the FPS
	float avg_fps = counted_frames / ( Timer_getTicks(fps_timer) / 1000.0f);
	// getTicks() returns the time in ms we convert it to seconds by dividing by 1000
	if(avg_fps > 2000000){ // if its ridiculously a large number we bring it down
		// This can happen on the first frame when much time has not passed
		avg_fps = 0;
	}

	// Additional buffer for storing the FPS
	char fps_buffer[8];
	// Store the avg fps in the buffer
	sprintf(fps_buffer, "%0.2f", avg_fps);
	// Create a new bstring
	bstring fps_text = bstrcpy(time_text);
	check(fps_text != NULL, "ERROR : Failed to create the fps text string!");
	// concatenate the fps buffer into the string
	bcatcstr(fps_text, fps_buffer);

	// Load up the texture with the font
	Texture_loadFromRenderedText(fps_text_texture, window, font, bdata(fps_text), text_color);

	// Destroy the bstring again
	bdestroy(fps_text);

error:
	return;
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
		
		// render the text on the screen
		Texture_render(fps_text_texture, window, (SCREEN_WIDTH - Texture_getWidth(fps_text_texture)) / 2, (SCREEN_HEIGHT - Texture_getHeight(fps_text_texture)) / 2, NULL);

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

