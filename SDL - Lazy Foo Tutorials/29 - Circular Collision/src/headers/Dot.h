#ifndef __DOT_H__
#define __DOT_H__

#include <SDL2/SDL.h>
#include <Texture.h>
#include <darray.h>
#include <dbg.h>

// A struct to define a circle
typedef struct{
	int x,y; // Defines the center of the circle and its radius
	int r;
}Circle;

typedef struct Dot{
	SDL_Point position;
	int x_velocity;
	int y_velocity;
	// Circular collider
	Circle* collider;
	
}Dot;

Dot* Dot_create(int x, int y); // intiializes the variables
void Dot_handleEvents(Dot* dot, SDL_Event* e);
void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot);

//  Collision Detection : We use the separating axis test to find if a collision has occured
// In this test we check for both the x and y projections of the objects and if they are not separated then
// they are colliding with each other
void Dot_move(Dot* dot, SDL_Rect* square, Circle* circle, int SCREEN_WIDTH, int SCREEN_HEIGHT); // checks for collision against the given object ( checks collision against an SDL_Rect here )
// new function for checking the collision between circles
bool checkCircleCollision(Circle* a, Circle* b);
// new function to check collisions between circle and a rectangle
bool checkCircleRectCollision(Circle* a, SDL_Rect* b);
// Get the collision boxes of the dot ( Circle now )
Circle* Dot_getCollider(Dot* dot);
// New Function to shift the collision boxes of the dot relative to its offset
void Dot_shiftColliders(Dot* dot);
// new function to caculate the distance squared between thwo points
// This is an optimization trick rather just depending on the simple distance
double distanceSquared(int x1, int y1, int x2, int y2);
#endif