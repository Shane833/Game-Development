#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
void handleEvents();
void update();
void render();
void clean();
// extra function to get the running variable value as it will be declared static
bool isRunning();

#endif