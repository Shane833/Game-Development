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

// Now handling the rumble is a little complicated as with the new version of the SDL
// came new APIs which made it easier to do the stuff.
// So there are basically two ways in which we can handle the haptics
// 1. We use the newly introduced SDL_GameController which handles both the joystick
// as well as the rumble/haptic of the controller, this interface is only compatible 
// with XBOX Controllers
// 2. Using the plaing old SDL_Joystick with SDL_Haptic and we would have to address
// both of them separately.

// Game controller 1 handler
SDL_GameController* gGameController = NULL; // 'g' means global
// Joystic 1 handler
SDL_Joystick* gJoystick = NULL;
// There is another datatype specifically to the haptic 
SDL_Haptic* gJoyHaptic = NULL;

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
	check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) >= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning : Linear Texture filtering not enabled!");
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	
	window = SDL_CreateWindow("Mouse Events and Buttons",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Now to check if any of the joysticks are connected
	check(SDL_NumJoysticks() > 0, "Warning: No JoySticks connected!"); // SDL_NumJoySticks() returns the no. of joysticks currently connected
	
	// Now to check if the joystick is game controller (XBOX Controller) compatible or not
	if(!SDL_IsGameController(0)){
		printf("Warning : Joystick is not game controller interface compatible! SDL_Error : %s\n", SDL_GetError());
	}
	else{
		// If its a game controller we will open it 
		gGameController = SDL_GameControllerOpen(0);
		// and now to check it supports rumble
		if(!SDL_GameControllerHasRumble(gGameController)){
			printf("Warning : Game Controller does not have rumble! SDL_Error : %s\n", SDL_GetError());
		}
		// By default its assumed that the game controller will have rumble but we can check for it manually too
	}
	// Now to load the joystick if the game controller could not be loaded
	if(gGameController == NULL){
		// Open the first joystick
		gJoystick = SDL_JoystickOpen(0);
		if(gJoystick == NULL){
			printf("Warning : Unable to open joystick! SDL_Error : %s\n", SDL_GetError());
		}
		else{
			// check if the joystick supports haptics
			if(!SDL_JoystickIsHaptic(gJoystick)){
				printf("Warning : Controller does not support haptics! SDL_Error : %s\n", SDL_GetError());
			}
			else{
				// Get Joystick Haptic Device
				gJoyHaptic = SDL_HapticOpenFromJoystick(gJoystick);
				if(gJoyHaptic == NULL){
					printf("Warning : Unable to get joystick haptics! SDL_Error: %s\n", SDL_GetError());
				}
				else{
					// Initialize the rumble
					if(SDL_HapticRumbleInit(gJoyHaptic) < 0){
						printf("Warning : Unable to initialize haptic rumble! SDL_Error: %s\n", SDL_GetError());
					}
				}
			}
			
		}
	}

	
	return true;
error:
	return false;
}

bool loadMedia()
{
	// First load the texture
	bool r = Texture_loadFromFile(renderer, &gArrowTexture, "Assets/splash.png");
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
		// Whenever a button is pressed
		else if(e.type == SDL_JOYBUTTONDOWN){
			// If its a XBOX Controller
			if(gGameController != NULL){
				// Play the rumble at 75% strength for 500ms
				if(SDL_GameControllerRumble(gGameController, 0xFFFF * 3/4, 0xFFFF * 3/4, 500) != 0){ // We are targetting the left and right motors individually
					printf("Warning : Unable to play game controller rumble! %s\n", SDL_GetError());
				}
			}
			// If we have simple haptics
			else if(gJoyHaptic != NULL){
				// Play the rumble at 75% strength for 500ms
				if(SDL_HapticRumblePlay(gJoyHaptic, 0.75, 500) != 0){
					printf("Warning : Unable to play haptic rumble! %s\n", SDL_GetError());
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
	
	// Closing the game controller or joystick with haptics
	if(gGameController != NULL){
		SDL_GameControllerClose(gGameController);
	}
	if(gJoyHaptic != NULL){
		SDL_HapticClose(gJoyHaptic);
	}
	if(gJoystick != NULL){
		SDL_JoystickClose(gJoystick);
	}

	gGameController = NULL;
	gJoystick = NULL;
	gJoyHaptic = NULL;
	
	// Close our window and the renderer
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

