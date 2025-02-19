// Now we will divide the whole process into functions
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

// Global Variables
// Window we'll be rendering to
SDL_Window* window = NULL;
// Window Surface we'll be drawing to
SDL_Surface* screenSurface = NULL;
// The image we will load and dispaly on the screen
SDL_Surface* tree = NULL;

// Function prototyping
// Starts up SDL and initalizes window
bool init(); 
// loads the media
bool loadMedia();
// Frees up the resources and closes SDL
void close();
 
int main(int argc, char* argv[]){

	// Calling the function on by one
	bool r = init();
	check(r == true, "Something went wrong!!!");
	
	r = loadMedia();
	check(r == true, "Something went wrong!!!");
	
	// Blitting means taking a copy on one thing and stamping
	// it on the the other 
	// src(1st) and dest(3rd) don't worry about the 2nd and 4th arg now
	SDL_BlitSurface(tree, NULL, screenSurface, NULL); // Applying the image
	
	// Updating the window surface
	SDL_UpdateWindowSurface(window);
	
	// Keeping the window alive
	SDL_Event e;
	bool quit = false;
	
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
	}
	
	/*
	We also declare an SDL_Event union. A SDL event is some thing like a key press, mouse motion, 
	joy button press, etc. In this application we're going to look for quit events to end the application.
	
	So we'll have the application loop while the user has not quit. This loop that keeps running
	while the application is active is called the main loop, which is sometimes called the game loop. 
	It is the core of any game application.
	
	At the top of our main loop we have our event loop. What this does is keep processing the event queue 
	until it is empty.
	When you press a key, move the mouse, or touch a touch screen you put events onto the event queue.
	
	The event queue will then store them in the order the events occurred waiting for you to process them. 
	When you want to find out what events occured so you can process them, you poll the event queue to get 
	the most recent event by calling SDL_PollEvent. What SDL_PollEvent does is take the most recent event 
	from the event queue and puts the data from the event into the SDL_Event we passed into the function.

	SDL_PollEvent will keep taking events off the queue until it is empty. When the queue is empty, 
	SDL_PollEvent will return 0. So what this piece of code does is keep polling events off the event 
	queue until it's empty. If an event from the event queue is an SDL_QUIT event 
	(which is the event when the user Xs out the window), we set the quit flag to true so 
	we can exit the application.
	*/
	
	
	// Freeing the resources
	close();
	
return 0;
error:
	return 1;
}

// Function definitions
bool init(){
	// Customizing function according to me 
	check(SDL_Init(SDL_INIT_VIDEO) >= 0,"Failed to initialize SDL! SDL_Error : %s\n", SDL_GetError());

	window = SDL_CreateWindow("Getting an Image on the Screen",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create the window! SDL_Error: %s", SDL_GetError());
	
	screenSurface = SDL_GetWindowSurface(window);
	// We call this function to retreive the pointer to the image inside the window
	// So that we can load up a image and show it up on the screen
	
	return true;
	error:
		return false;
}

bool loadMedia(){
	// Loads up the image 
	tree = SDL_LoadBMP("Assets/tree.bmp");
	check(tree != NULL,"Falied to load tree.bmp! SDL_Error: %s\n", SDL_GetError());
	
	return true;
	error:
		return false;
}

void close(){
	// Deallocate the surface
	SDL_FreeSurface(tree);
	tree = NULL;
	
	// Destroy the window
	SDL_DestroyWindow(window);
	window = NULL;
	
	// Quit SDL Subsystems
	SDL_Quit();
}
	
	
