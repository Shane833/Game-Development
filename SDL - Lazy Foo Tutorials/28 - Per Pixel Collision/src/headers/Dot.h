#ifndef __DOT_H__
#define __DOT_H__

#include <SDL2/SDL.h>
#include <Texture.h>
#include <darray.h>
#include <dbg.h>

typedef struct Dot{
	SDL_Point position;
	int x_velocity;
	int y_velocity;
	// Collision Boxes
	DArray* colliders;
}Dot;

Dot* Dot_create(int x, int y); // intiializes the variables
void Dot_handleEvents(Dot* dot, SDL_Event* e);
void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot);

//  Collision Detection : We use the separating axis test to find if a collision has occured
// In this test we check for both the x and y projections of the objects and if they are not separated then
// they are colliding with each other
void Dot_move(Dot* dot, DArray* otherColliders, int SCREEN_WIDTH, int SCREEN_HEIGHT); // checks for collision against the given object
// new function for checking the collision between boxes
bool checkCollision(SDL_Rect* a, SDL_Rect* b);
// new function to check the collision between a set of colliders
bool checkMultipleCollisions(DArray* colliders_a, DArray* colliders_b);

// Get the collision boxes of the dot
DArray* Dot_getColliders();

// New Function to shift the collision boxes of the dot relative to its offset
void Dot_shiftColliders(Dot* dot);
#endif