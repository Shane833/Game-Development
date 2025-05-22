#ifndef __TILE_H__
#define __TILE_H__

#include <SDL2/SDL.h>
#include <Texture.h>
#include <dbg.h>

// Tile Constants
#define TILE_WIDTH 80
#define TILE_HEIGHT 80
#define TOTAL_TILES 192
#define TOTAL_TILE_SPRITES 12

// Different Tile Sprites
extern const int TILE_RED;
extern const int TILE_GREEN;
extern const int TILE_BLUE;
extern const int TILE_CENTER;
extern const int TILE_TOP;
extern const int TILE_TOPRIGHT;
extern const int TILE_RIGHT;
extern const int TILE_BOTTOMRIGHT;
extern const int TILE_BOTTOM;
extern const int TILE_BOTTOMLEFT;
extern const int TILE_LEFT;
extern const int TILE_TOPLEFT;

// Tile Texture
extern Texture tileTexture;
// Tile Clips
extern SDL_Rect tileClips[];

// Defines the Tiles
typedef struct{
	SDL_Rect box;
	int type;
}Tile;

Tile* Tile_create(int x, int y, int tileType); // creates the tile
void Tile_render(SDL_Renderer* renderer, Tile* tile, SDL_Rect* camera); // renders the tile to the screen

#endif