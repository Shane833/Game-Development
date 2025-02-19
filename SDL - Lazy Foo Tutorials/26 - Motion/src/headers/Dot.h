#ifndef __DOT_H__
#define __DOT_H__

#include <SDL2/SDL.h>
#include <Texture.h>
#include <dbg.h>

typedef struct Dot{
	SDL_Point position;
	int x_velocity;
	int y_velocity;
}Dot;

Dot* Dot_create(); // intiializes the variables
void Dot_handleEvents(Dot* dot, SDL_Event* e);
void Dot_move(Dot* dot);
void Dot_render(SDL_Renderer* renderer, Texture* texture, Dot* dot);
#endif