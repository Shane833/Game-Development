#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <dbg.h>

// This is our Box Colllider
typedef SDL_Rect Box_Collider;

// New Box Collider with floating points
typedef SDL_FRect Box_FCollider;

// This is our Circle Collider
typedef struct{
	int x,y; // Defines the center of the circle and its radius
	int r;
}Circle_Collider;

// Simple Box Collision
bool checkBoxCollision(const Box_Collider* a, const Box_Collider* b);
// Simple Floating Box Collision
bool checkBoxFCollision(const Box_FCollider* a, const Box_FCollider* b);
// Simple Circle Collision
bool checkCircleCollision(const Circle_Collider* a, const Circle_Collider* b);
// Box-Circle Collision
bool checkCircleBoxCollision(const Circle_Collider* a, const Box_Collider* b);


#endif