#include <Particle.h>

Particle* Particle_create(int x, int y)
{
	Particle* temp = calloc(1, sizeof(Particle));
	check(temp != NULL, "ERROR : Failed to create the particle!");
	
	// Provide random position to the particle
	temp->position.x = x - 5 + (rand() % 25);
	temp->position.y = y - 5 + (rand() % 25);
	// Provide a random frame time for different life
	temp->frame = rand() % 5;
	// Provide a random texture to the particle
	switch(rand() % 3){
		case 0: temp->p_texture = RED; break;
		case 1: temp->p_texture = BLUE; break;
		case 2: temp->p_texture = GREEN; break;
	}

	return temp;
error:
	return NULL;
}

void Particle_render(SDL_Renderer* renderer, Texture* texture_array, Particle* p)
{
	check(p != NULL, "ERROR : Invalid Particle!");
	
	// First we render the particle itself
	Texture_render(renderer, &(texture_array[p->p_texture]), p->position.x, p->position.y, NULL);
	
	// Then if the no. of frames are even we will also render the shimmer texture
	if(p->frame % 2 == 0){
		Texture_render(renderer, &(texture_array[SHIMMER]), p->position.x, p->position.y, NULL);
	}
	
	// increment the no. of frames that it has been on the screen
	p->frame++;
error:
	return;
}

bool Particle_isDead(Particle* p)
{
	// If the particle has persisted for more than 10 frames we mark it as dead
	return p->frame > 10;
}

void Particle_destroy(Particle* p)
{
	if(p)
	free(p);
}
