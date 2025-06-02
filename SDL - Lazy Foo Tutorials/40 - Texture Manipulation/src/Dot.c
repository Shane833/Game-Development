#include <Dot.h>


// Global variables
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;
const int DOT_VEL = 5; // reducing the velocity so the effect is more visible

// Dot Texture
Texture dotTexture;

// Function Defintions
Dot* Dot_create(int x, int y)
{
	Dot* new_dot = (Dot*)malloc(sizeof(Dot));
	check(new_dot != NULL, "Failed to create the dot!");
	
	new_dot->position.x = x;
	new_dot->position.y = y;
	new_dot->x_velocity = 0;
	new_dot->y_velocity = 0;

	new_dot->box.x = x;
	new_dot->box.y = y;
	new_dot->box.w = DOT_WIDTH;
	new_dot->box.h = DOT_HEIGHT;
	
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

void Dot_move(Dot* dot, Tile* tiles[], int LEVEL_WIDTH, int LEVEL_HEIGHT)
{
	check(dot != NULL, "Inalid Dot!");
	
	// move the dot to the left or right
	dot->position.x += dot->x_velocity;
	dot->box.x = dot->position.x;
	
	// If the dot collided or went too far to the left or right or touched a wall
	if( (dot->position.x < 0) || (dot->position.x + DOT_WIDTH > LEVEL_WIDTH) || Dot_touchesWall(dot, tiles) ){
		// move back
		dot->position.x -= dot->x_velocity;
		dot->box.x = dot->position.x;
	}
	
	// move the do to the up or down or touched a wall
	dot->position.y += dot->y_velocity;	
	dot->box.y = dot->position.y;

	if( (dot->position.y < 0) || (dot->position.y + DOT_HEIGHT > LEVEL_HEIGHT) || Dot_touchesWall(dot, tiles) ){
		// move back
		dot->position.y -= dot->y_velocity;
		dot->box.y = dot->position.y;
	}
	
error:  // fallthrough
	return;
}

void Dot_setCamera(Dot* dot, SDL_Rect* camera, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const int LEVEL_WIDTH, const int LEVEL_HEIGHT)
{
	// center the camera over the dot
	camera->x = (dot->box.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera->y = (dot->box.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;	

	// Keep the camera in bounds
	if(camera->x < 0){
		camera->x = 0;
	}
	if(camera->y < 0){
		camera->y = 0;
	}
	if(camera->x > LEVEL_WIDTH - camera->w){
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if(camera->y > LEVEL_HEIGHT - camera->h){
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}

void Dot_render(SDL_Renderer* renderer, SDL_Rect* camera, Texture* dot_texture, Dot* dot)
{
	check(dot != NULL, "Invalid Dot!");
	
	Texture_render(renderer, dot_texture, dot->box.x - camera->x, dot->box.y - camera->y, NULL);
	// By subtracting the camX and camY points we make sure that we keep the dot inside the 
	// frame of the camera i.e. we are basically changing the origin of the dot from (0,0) to (camX, camY)
	
	
error: // fallthrough
	return;
}

void Dot_destroy(Dot* dot)
{
	if(dot){
		free(dot);
	}
}

// function to check collision with the tile walls
bool Dot_touchesWall(Dot* dot, Tile* tiles[])
{
	// we will simple iterate over the tiles
	for(int i = 0; i < TOTAL_TILES; i++){
		// If the tile happens to be a wall
		if(tiles[i]->type >= TILE_CENTER && tiles[i]->type <= TILE_TOPLEFT){
			// if the dot touches the wall tiles
			if (checkBoxCollision(&(dot->box), &(tiles[i]->box)) ){
				return true;
			}
		}
	}

	return false;
}
