#include <SDL2/SDL.h>
#include <Game.h>
#include <dbg.h>

int main(int argc, char* argv[])
{
	bool rc = init("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	check(rc == true, "Failed to Initialize");
	
	while(isRunning()){
		handleEvents();
		render();
	}
	
	clean();
	
	return 0;
	error:
		return 1;
}