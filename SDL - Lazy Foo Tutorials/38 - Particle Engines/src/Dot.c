#include <Dot.h>

// Global variables
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;
const int DOT_VEL = 5; // reducing the velocity so the effect is more visible

// Function Defintions
Dot* Dot_create(int x, int y)
{
	Dot* new_dot = (Dot*)malloc(sizeof(Dot));
	check(new_dot != NULL, "Failed to create the dot!");
	
	new_dot->position.x = x;
	new_dot->position.y = y;
	new_dot->x_velocity = 0;
	new_dot->y_velocity = 0;
	
	// Initialize the particles
	for(int i = 0;i < TOTAL_PARTICLES;i++){
		new_dot->particles[i] = Particle_create(x,y);
	}
	
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

void Dot_move(Dot* dot, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	check(dot != NULL, "Inalid Dot!");
	
	// move the dot to the left or right
	dot->position.x += dot->x_velocity;
	
	// If the dot collided or went too far to the left or right
	if( (dot->position.x < 0) || (dot->position.x + DOT_WIDTH > SCREEN_WIDTH) ){
		// move back
		dot->position.x -= dot->x_velocity;
	}
	
	// move the do to the up or down
	dot->position.y += dot->y_velocity;	
	
	if( (dot->position.y < 0) || (dot->position.y + DOT_HEIGHT > SCREEN_HEIGHT) ){
		// move back
		dot->position.y -= dot->y_velocity;
	}
	
error:  // fallthrough
	return;
}

void Dot_render(SDL_Renderer* renderer, Texture* dot_texture, Texture* texture_array, Dot* dot)
{
	check(dot != NULL, "Invalid Dot!");
	
	Texture_render(renderer, dot_texture, dot->position.x, dot->position.y, NULL);
	// By subtracting the camX and camY points we make sure that we keep the dot inside the 
	// frame of the camera i.e. we are basically changing the origin of the dot from (0,0) to (camX, camY)
	
	// After rendering the dot we will render the particles as we want them on top
	Dot_renderParticles(renderer, texture_array, dot); 
	
error: // fallthrough
	return;
}

void Dot_renderParticles(SDL_Renderer* renderer, Texture* texture_array, Dot* dot)
{
	check(dot != NULL, "ERROR : Invalid Dot!");
	
	// We will go through each of the particles
	for(int i = 0;i < TOTAL_PARTICLES; i++){
		// Check if its dead
		if(Particle_isDead(dot->particles[i])){
			// If its dead then destroy it and create a new one
			Particle_destroy(dot->particles[i]);
			dot->particles[i] = Particle_create(dot->position.x, dot->position.y);
		}
	}
	
	// And at last we display all the particles
	for(int i = 0;i < TOTAL_PARTICLES;i++){
		Particle_render(renderer, texture_array, dot->particles[i]);
	}

error: // Fallthrough
	return;
}

void Dot_destroy(Dot* dot)
{
	if(dot){
		for(int i = 0;i < TOTAL_PARTICLES;i++){
			Particle_destroy(dot->particles[i]);
		}
		free(dot);
	}
}
