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

// Similar how to we handled the mouse events
// we can obtain the state of the keyboard instead of relying on the events
// However it is still necessary to have our event loop since the scan codes
// are updated every event loop and make sure you poll all the events before 
// checking the key states i.e. the key states will be checked out of the event loop

// Texture Variable
Texture up_texture;
Texture down_texture;
Texture left_texture;
Texture right_texture;
Texture press_texture;

Texture* current_texture; // a pointer to reference the different texture as per our needs

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
	bool r = Texture_loadFromFile(renderer, &up_texture, "Assets/up.png");
	check(r != false, "Failed to load the button texture");
	
	r = Texture_loadFromFile(renderer, &down_texture, "Assets/down.png");
	check(r != false, "Failed to load the button texture");
	
	r = Texture_loadFromFile(renderer, &left_texture, "Assets/left.png");
	check(r != false, "Failed to load the button texture");
	
	r = Texture_loadFromFile(renderer, &right_texture, "Assets/right.png");
	check(r != false, "Failed to load the button texture");
	
	r = Texture_loadFromFile(renderer, &press_texture, "Assets/press.png");
	check(r != false, "Failed to load the button texture");
	
	// initialize the current texture
	current_texture = &press_texture;
	
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
	
	// Set texture based on current keystate 
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL); // Retreives a pointer to the key state array
	
	// Scan codes are like keycodes but they are better suited for international keyboards
	if(current_key_states[SDL_SCANCODE_UP]) current_texture = &up_texture;
	else if(current_key_states[SDL_SCANCODE_DOWN]) current_texture = &down_texture;
	else if(current_key_states[SDL_SCANCODE_LEFT]) current_texture = &left_texture;
	else if(current_key_states[SDL_SCANCODE_RIGHT]) current_texture = &right_texture;
	else current_texture = &press_texture;
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// render the current texture
	Texture_render(renderer, current_texture, 0, 0, NULL);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
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
	IMG_Quit();
	SDL_Quit();
}

