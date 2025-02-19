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

// Now similar to how SDL can handle mouse and keyboard, so can we for the Gamepad/controller/joysticks etc.

// Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;
// Creating a deadzone is necessary as the SDL takes input between -32768 to 32767
// hence even slight tap on the joystick can mean that it'll report a reading of 1000+

// Game controller 1 handler
SDL_Joystick* gGameController = NULL; // 'g' means global
// Here we use the SDL_JoyStick datatype to control the controller

// Texture Variable
Texture gArrowTexture;

// Normalized directions
int xDir = 0;
int yDir = 0;
// now we will only take into account whether the joystick is in the x or y direction
// we are not concerned with the precision. So we will only set the direction to be
// x == -1 (LEFT), x == 1 (RIGHT), y == 1 (UP), y == -1(DOWN)
// else 0 would mean it is in the CENTER

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
	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning : Linear Texture filtering not enabled!");
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); // returns 0 on success and -1 on failure
	
	window = SDL_CreateWindow("Mouse Events and Buttons",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Now to check if any of the joysticks are connected
	check(SDL_NumJoysticks() > 0, "Warning: No JoySticks connected!"); // SDL_NumJoySticks() returns the no. of joysticks currently connected
	// Now to load the joystick if it exists
	gGameController = SDL_JoystickOpen(0); // This function can be used to reference the joysticks and will now report events to the SDL_Event queue
	check(gGameController != NULL, "Warning: Unable to open game controller! SDL_Error: %s\n", SDL_GetError());
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	// First load the texture
	bool r = Texture_loadFromFile(renderer, &gArrowTexture, "Assets/arrow.png");
	check(r != false, "Failed to load the button texture");
	
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
		// checking for the axis motion
		else if(e.type == SDL_JOYAXISMOTION){
			// Checking for the motion on controller 0
			if(e.jaxis.which == 0){
				// X axis motion
				if(e.jaxis.axis == 0){
					// Left of the dead zone
					if(e.jaxis.value < -JOYSTICK_DEAD_ZONE){
						xDir = -1;
					}
					// Right of the dead zone
					else if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
						xDir = 1;
					}
					else xDir = 0;
				}
				// Y axis motion
				if(e.jaxis.axis == 1){
					// Below the dead zone
					if(e.jaxis.value < -JOYSTICK_DEAD_ZONE){
						yDir = -1;
					}
					// Above the dead zone
					else if(e.jaxis.value > JOYSTICK_DEAD_ZONE){
						yDir = 1;
					}
					else yDir = 0;
				}
			}
		}
	}	
	
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Now to calculate the angle using the x and y
	double joystickAngle = atan2((double)yDir, (double)xDir) * (180 / M_PI); 
	// using atan2 or inverse tan 2 or arc tan 2 we convert the x and y coordinated to angle in radians
	// and then again to convert the angle to degrees we multiply by 180 / PI
	// This provide an 8 way movement
	// Correcting the angle
	if(xDir == 0 && yDir == 0){ // when both of them are 0, we could get a garbage angle and hence we correct it
		joystickAngle = 0;
	}
	
	// render the current texture with the given angle
	Texture_renderEx(renderer, &gArrowTexture, (SCREEN_WIDTH - Texture_getWidth(&gArrowTexture)) / 2, (SCREEN_HEIGHT - Texture_getHeight(&gArrowTexture)) / 2, NULL, joystickAngle, NULL, SDL_FLIP_NONE);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	// Free up the texture
	Texture_destroy(&gArrowTexture);
	
	// Closing the game controller
	SDL_JoystickClose(gGameController);
	
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

