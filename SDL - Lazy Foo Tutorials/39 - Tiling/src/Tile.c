#include <Tile.h>

// Different Tile Sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

// Tile Texture
Texture tileTexture;
// Tile clips
SDL_Rect tileClips[TOTAL_TILE_SPRITES]; // We can't declare an array with variable size even if its const

// Creating the Tile
Tile* Tile_create(int x, int y, int tileType)
{
	Tile* temp = malloc(sizeof(Tile));
	check(temp != NULL, "ERROR : Failed to create the tile!");
	
	temp->box.x = x;
	temp->box.y = y;
	
	temp->box.w = TILE_WIDTH;
	temp->box.h = TILE_HEIGHT;
	
	temp->type = tileType;
	
	return temp;
error:
	return NULL;
}

// Rendering the Tile
void Tile_render(SDL_Renderer* renderer, Texture* tileTexture, Tile* tile, SDL_Rect* camera)
{
	// If the tile is on the screen
	if( checkBoxCollision((Box_Collider*)camera, &(tile->box)) ){
		// show the tile
		Texture_render(renderer, tileTexture, tile->box.x - camera->x, tile->box.y - camera->y, &tileClips[tile->type]);
	}
}

