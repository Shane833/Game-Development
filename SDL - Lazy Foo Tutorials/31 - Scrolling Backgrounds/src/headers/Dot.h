#ifndef __DOT_H__
#define __DOT_H__

#include <SDL2/SDL.h>
#include <Texture.h>
#include <dbg.h>

extern const int DOT_WIDTH;
extern const int DOT_HEIGHT;
extern const int DOT_VEL;

typedef struct Dot{
	SDL_Point position;
	int x_velocity;
	int y_velocity;
}Dot;

Dot* Dot_create(int x, int y); // intiializes the variables
void Dot_handleEvents(Dot* dot, SDL_Event* e);
void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot);
//  Collision Detection : We use the separating axis test to find if a collision has occured
// In this test we check for both the x and y projections of the objects and if they are not separated then
// they are colliding with each other
void Dot_move(Dot* dot, int SCREEN_WIDTH, int SCREEN_HEIGHT); // checks for collision against the given object ( checks collision against an SDL_Rect here )

#endif