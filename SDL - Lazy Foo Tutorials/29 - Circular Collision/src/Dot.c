#include <Dot.h>

// Global variables
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;
static const int DOT_VEL = 1; // reducing the velocity so the effect is more visible

// Function Defintions
Dot* Dot_create(int x, int y)
{
	Dot* new_dot = (Dot*)malloc(sizeof(Dot));
	check(new_dot != NULL, "Failed to create the dot!");
	
	new_dot->position.x = x;
	new_dot->position.y = y;
	new_dot->x_velocity = 0;
	new_dot->y_velocity = 0;
	
	// Initializing the circular collider
	new_dot->collider = (Circle*)malloc(sizeof(Circle));
	check(new_dot->collider != NULL, "Failed to create the collider!");
	new_dot->collider->r = DOT_WIDTH / 2;
	
	
	Dot_shiftColliders(new_dot);
	
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

void Dot_move(Dot* dot, SDL_Rect* square, Circle* circle, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	check(dot != NULL, "Inalid Dot!");
	check(square != NULL, "Invalid Rect!");
	check(circle != NULL, "Invalid circle collider!");
	
	// move the dot to the left or right
	dot->position.x += dot->x_velocity;
	Dot_shiftColliders(dot); // update the collider's position
								  // Using the new collision function
	// If the dot collided or went too far to the left or right
	if( (dot->position.x - dot->collider->r < 0) || (dot->position.x + dot->collider->r > SCREEN_WIDTH) || (checkCircleCollision(dot->collider, circle)) || (checkCircleRectCollision(dot->collider, square)) ){
		// move back
		dot->position.x -= dot->x_velocity;
		Dot_shiftColliders(dot); // update the collider's position
	}
	
	// move the do to the up or down
	dot->position.y += dot->y_velocity;	
	Dot_shiftColliders(dot);
	
	if( (dot->position.y - dot->collider->r < 0) || (dot->position.y + dot->collider->r > SCREEN_HEIGHT) || (checkCircleCollision(dot->collider, circle)) || (checkCircleRectCollision(dot->collider,square)) ){
		// move back
		dot->position.y -= dot->y_velocity;
		Dot_shiftColliders(dot);
	}
	
error:  // fallthrough
	return;
}

void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot)
{
	check(dot != NULL, "Invalid Dot!");
	
	Texture_render(renderer, texture, dot->position.x - dot->collider->r, dot->position.y - dot->collider->r, NULL);

error: // fallthrough
	return;
}

bool checkCollision(SDL_Rect* a, SDL_Rect* b)
{
	// The sides of the rectangle
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	
	// calculate the sides of the rect A
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;
	
	// calculate the sides of the rect B
	leftB = b->x;
	rightB  = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;
	
	// Now we do our separating axis test and if any of the axis from 
	// either of the boxes are separate then there is not collision else
	// they will collide

	if(bottomA <= topB) return false;
	if(topA >= bottomB) return false;
	if(rightA <= leftB) return false;
	if(leftA >= rightB) return false;
	
	// if none of the sides of A are outside B then they are colliding
	return true;
}

// Best one to make you understand about collisions
// https://stackoverflow.com/questions/31022269/collision-detection-between-two-rectangles-in-java
/*
Say you have Rect A, and Rect B. Proof is by contradiction. Any one of four conditions guarantees that no overlap can exist:

Cond1. If A's left edge is to the right of the B's right edge, - then A is Totally to right Of B
Cond2. If A's right edge is to the left of the B's left edge, - then A is Totally to left Of B
Cond3. If A's top edge is below B's bottom edge, - then A is Totally below B
Cond4. If A's bottom edge is above B's top edge, - then A is Totally above B
So condition for Non-Overlap is

Cond1 Or Cond2 Or Cond3 Or Cond4

Therefore, a sufficient condition for Overlap is the opposite (De Morgan)

Not Cond1 And Not Cond2 And Not Cond3 And Not Cond4 This is equivalent to:

A's Left Edge to left of B's right edge, and
A's right edge to right of B's left edge, and
A's top above B's bottom, and
A's bottom below B's Top
Note 1: It is fairly obvious this same principle can be extended to any number of dimensions. 
Note 2: It should also be fairly obvious to count overlaps of just one pixel, change the < and/or the > 
on that boundary to a <= or a >=.
*/

// New Function Definitions

// Retreives the colliders of the 
Circle* Dot_getCollider(Dot* dot)
{
	return dot->collider;
}

// Function to update the colliders as per the movement of the dot
void Dot_shiftColliders(Dot* dot)
{
	check(dot != NULL, "ERROR : Invalid dot!");
	
	dot->collider->x = dot->position.x;
	dot->collider->y = dot->position.y;
	
error:
	return;
}

// New Function to check collision between two cirlces
bool checkCircleCollision(Circle* a, Circle* b)
{
	check(a != NULL, "ERROR : Invalid Circle!!!");
	check(b != NULL, "ERROR : Invalid Circle!!!");
	// Calculate the total radius squared
	int totalRadiusSquared = a->r + b->r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;
	
	// If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(a->x, a->y, b->x, b->y) < (totalRadiusSquared)){
		// The circles have collided
		return true;
	}

error: // fallthrough
	// If not
	return false;
}

// New Function to check collision between a circle and a rectangle
bool checkCircleRectCollision(Circle* a, SDL_Rect* b)
{
	check(a != NULL, "ERROR : Invalid Circle!!!");
	check(b != NULL, "ERROR : Invalid Rectangle!!!");
	
	// Closest point on the collision box (rectangle)
	int cX, cY;
	
	// Find the closest x offset
	if(a->x < b->x){
		cX = b->x;
	} else if (a->x > b->x + b->w){
		cX = b->x + b->w;
	}else{
		cX = a->x;
	}
	
	// Find the closest y offset
	if (a->y < b->y){
		cY = b->y;
	} else if (a->y > b->y + b->h){
		cY = b->y + b->h;
	} else{
		cY = a->y;
	}
	
	// If the closest point is inside the circle
	if (distanceSquared(a->x, a->y, cX, cY) < a->r * a->r){
		// This box and cirlce have collided
		return true;
	}
	
error: // fallthrough
	// If there is no collision
	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	
	return ((deltaX * deltaX) + (deltaY * deltaY));
}
