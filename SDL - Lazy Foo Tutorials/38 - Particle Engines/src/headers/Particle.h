#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <Texture.h>

// Particles are just mini-animations we can simply spawn them around for the effects

typedef struct{
	SDL_Point position;
	int frame;
	Texture* texture;
}Particle;

Particle* Particle_create(int x, int y);
void Particle_render(Particle* p);
bool Particle_isDead(Particle* p);

#endif