#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <Texture.h>

// Particles are just mini-animations we can simply spawn them around for the effects

typedef enum Particle_texture{
	RED,
	BLUE,
	GREEN,
	SHIMMER
}Particle_texture;

typedef struct{
	SDL_Point position; // Position of the particle
	int frame; // No. of frames it lives on the screen
	Particle_texture p_texture; 
}Particle;

Particle* Particle_create(int x, int y);
void Particle_render(SDL_Renderer* renderer, Texture* texture_array, Particle* p);
// We obtain the array of texture and depending on the texture type we render that texture
// from the texture array
bool Particle_isDead(Particle* p);
void Particle_destroy(Particle* p);

#endif