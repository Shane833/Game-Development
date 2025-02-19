#include <Dot.h>

// Global variables
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;
static const int DOT_VEL = 10; // moves 10 pixels/frame

// Function Defintions
Dot* Dot_create()
{
	Dot* new_dot = (Dot*)malloc(sizeof(Dot));
	check(new_dot != NULL, "Failed to create the dot!");
	
	new_dot->position.x = 0;
	new_dot->position.y = 0;
	new_dot->x_velocity = 0;
	new_dot->y_velocity = 0;
	
	return new_dot;
error:
	return NULL;
}

void Dot_handleEvents(Dot* dot, SDL_Event* e)
{
	check(dot != NULL, "Invalid Dot!");
	
	// if a key was pressed
	if(e->type == SDL_KEYDOWN && e->key.repeat == 0){
		// adjust the velocity, here we keep on adding to the velocity
		switch(e->key.keysym.sym){
			case SDLK_UP: dot->y_velocity -= DOT_VEL; break; 
			case SDLK_DOWN: dot->y_velocity += DOT_VEL; break; 
			case SDLK_LEFT: dot->x_velocity -= DOT_VEL; break; 
			case SDLK_RIGHT: dot->x_velocity += DOT_VEL; break; 
		}
	}
	
	// if a key was released
	if(e->type == SDL_KEYUP && e->key.repeat == 0){
		// adjust the velocity, here redo the changes and put the velocity back to zero
		switch(e->key.keysym.sym){
			case SDLK_UP: dot->y_velocity += DOT_VEL; break; 
			case SDLK_DOWN: dot->y_velocity -= DOT_VEL; break; 
			case SDLK_LEFT: dot->x_velocity += DOT_VEL; break; 
			case SDLK_RIGHT: dot->x_velocity -= DOT_VEL; break; 
		}
	}
	
error: // fallthrough
	return;
}

void Dot_move(Dot* dot)
{
	check(dot != NULL, "Inalid Dot!");
	
	// move the dot to the left or right
	dot->position.x += dot->x_velocity;
	
	if((dot->position.x + DOT_WIDTH > 640) || (dot->position.x < 0)){
		dot->position.x -= dot->x_velocity;
	}
	
	// move the do to the up or down
	dot->position.y += dot->y_velocity;	
	
	if((dot->position.y + DOT_HEIGHT > 480) || (dot->position.y < 0)){
		dot->position.y -= dot->y_velocity;
	}
	

error:  // fallthrough
	return;
}

void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot)
{
	check(dot != NULL, "Invalid Dot!");
	
	Texture_render(renderer, texture, dot->position.x, dot->position.y, NULL);

error: // fallthrough
	return;
}