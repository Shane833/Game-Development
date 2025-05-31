#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <Window.h>
#include <Texture.h>
#include <dbg.h>
#include <Dot.h>

// Usual Global Variables
extern Window* window;
extern SDL_Renderer* renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern TTF_Font* font; // globally used font
extern bool quit;

// Functions
bool init(); // Intializes subsystems and 
bool loadMedia(Tile* tiles[]); // Loads the textures, music etc
void handleEvents(); // handles the user and game event
void update(); // Handles physics and other updates
void render(); // Display the sprites and textures on the screen
void close(Tile* tiles[]); // frees the resources and closes the subsystems
int run();

// Additional Functions
// Sets tiles from the tile map
bool setTiles(Tile* tiles[]);
void freeTiles(Tile* tiles[]);

#endif