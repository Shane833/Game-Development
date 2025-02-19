#include <Game.h>
#include <Texture.h>
#include <UI.h>

#define TOTAL_BUTTONS 4

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;
	
// Texture Variable
Texture button_texture; // Button Texture
SDL_Rect button_sprites[BUTTON_SPRITE_TOTAL]; // array of differnt clips indicating the different sprites
Button buttons[TOTAL_BUTTONS]; // buttons duh!

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
	bool r = Texture_loadFromFile(renderer, &button_texture, "Assets/button.png");
	check(r != false, "Failed to load the button texture");
	
	// Initialize each of the buttons
	for(int i = 0; i < TOTAL_BUTTONS; i++){
		Button* temp = Button_create(300,200);
		check(temp != NULL, "Failed to create the button");
		buttons[i] = *temp; // Putting the newly created button into the button array
	}
	
	// now to set the position of each of the buttons
	buttons[1].position.x = 320;
	buttons[1].position.y = 0;
	
	buttons[2].position.x = 0;
	buttons[2].position.y = 240;
	
	buttons[3].position.x = 320;
	buttons[3].position.y = 240;
	
	button_sprites[BUTTON_SPRITE_MOUSE_OUT].x = 0;
	button_sprites[BUTTON_SPRITE_MOUSE_OUT].y = 0;
	button_sprites[BUTTON_SPRITE_MOUSE_OUT].w = 300;
	button_sprites[BUTTON_SPRITE_MOUSE_OUT].h = 200;
	
	button_sprites[BUTTON_SPRITE_MOUSE_OVER_MOTION].x = 0;
	button_sprites[BUTTON_SPRITE_MOUSE_OVER_MOTION].y = 200;
	button_sprites[BUTTON_SPRITE_MOUSE_OVER_MOTION].w = 300;
	button_sprites[BUTTON_SPRITE_MOUSE_OVER_MOTION].h = 200;
	
	button_sprites[BUTTON_SPRITE_MOUSE_DOWN].x = 0;
	button_sprites[BUTTON_SPRITE_MOUSE_DOWN].y = 400;
	button_sprites[BUTTON_SPRITE_MOUSE_DOWN].w = 300;
	button_sprites[BUTTON_SPRITE_MOUSE_DOWN].h = 200;
	
	button_sprites[BUTTON_SPRITE_MOUSE_UP].x = 0;
	button_sprites[BUTTON_SPRITE_MOUSE_UP].y = 600;
	button_sprites[BUTTON_SPRITE_MOUSE_UP].w = 300;
	button_sprites[BUTTON_SPRITE_MOUSE_UP].h = 200;

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
		// now to hand the event for each of the button
		for(int i = 0;i < TOTAL_BUTTONS; i++){
			Button_handleMouseEvents(&buttons[i], &e);
		}
	}		
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
			
	// Rendering the buttons
	for(int i = 0;i < TOTAL_BUTTONS; i++){
		Button_render(renderer, &buttons[i], &button_texture, button_sprites);
	}
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	Texture_destroy(&button_texture);
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

