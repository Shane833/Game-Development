#include <Collider.h>

// Function for optimizing circle collision check
static double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	
	return ((deltaX * deltaX) + (deltaY * deltaY));
}

// Box-Box Collision
bool checkBoxCollision(const Box_Collider* a, const Box_Collider* b)
{
	check(a != NULL, "ERROR : Invalid Box_Collider!");
	check(b != NULL, "ERROR : Invalid Box_Collider!");

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
error:
	return false;
}

// Circle-Circle Collision
bool checkCircleCollision(const Circle_Collider* a, const Circle_Collider* b)
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

// Box-Circle Collision
bool checkCircleBoxCollision(const Circle_Collider* a, const Box_Collider* b)
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