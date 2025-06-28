#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <Window.h>
#include <Texture.h>
#include <dbg.h>
#include <BitmapFont.h>

// Usual Global Variables
Window * window = NULL;
SDL_Renderer * renderer = NULL;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int LEVEL_WIDTH;
const int LEVEL_HEIGHT;
TTF_Font * font = NULL; // globally used font
bool quit = false;

// Functions
bool init(); // Intializes subsystems and 
bool loadMedia(); // Loads the textures, music etc
void handleEvents(); // handles the user and game event
void update(); // Handles physics and other updates
void render(); // Display the sprites and textures on the screen
void close(); // frees the resources and closes the subsystems
int run();

#endif