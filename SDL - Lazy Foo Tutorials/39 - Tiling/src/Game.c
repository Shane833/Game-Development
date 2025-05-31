#include <Game.h>


// Usual Global Variables
Window* gWindow; 
Dot* dot;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

TTF_Font* font = NULL;
bool quit = false;

// Array of tile pointers (don't forget to initialize it)
// So I was wrong in freeing the memory of the objects allocated in this array
// Since its declared on the stack its lifetime and hence deallocation will be handled
// on the stack itself
Tile* tiles[TOTAL_TILES] = { NULL };

SDL_Rect camera = {0,0,640,480};

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia(tiles);
	check(r == true, "Something went wrong");
	
	while(!quit)
	{	
		handleEvents();
		update();
		render();	
	}
	
	return 0;
error:
	return 1;
}

int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	
	// Destroy the dot
	Dot_destroy(dot);
	
	// Close all of our windows
	Window_destroy(gWindow);	
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_VIDEO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	gWindow = Window_create(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(gWindow != NULL, "ERROR : Failed to create window");
	
	dot = Dot_create(0, 0);
	check(dot != NULL, "ERROR : Failed to create the dot!");

	
	return true;
error:
	return false;
}

// Loading the tiles
bool setTiles(Tile* tiles[])
{
	// tile offsets
	int x = 0, y = 0;

	// open the map
	FILE* map = fopen("Assets/lazy.map","r");
	check(map != NULL, "ERROR: Failed to load the file!, SDL Error : %s",SDL_GetError());

	// Initialize the tiles
	for(int i = 0; i < TOTAL_TILES; i++){
		// determine what kind of tile will be made
		int tile_type = -1; // depiciting none of the types

		// reading from the file
		size_t rc = fscanf(map, "%d", &tile_type);
		check(rc <= 1, "ERROR : Failed to read from the file!, SDL Error : %s",SDL_GetError());

		// now to check if the tile type is valid
		check( ( (tile_type >= 0) && (tile_type <= TOTAL_TILE_SPRITES) ), "ERROR : Invalid Tile Type!");
		tiles[i] = Tile_create(x, y, tile_type);
		check(tiles[i] != NULL, "ERROR : Failed to create a tile!, SDL Error : %s", SDL_GetError());
		
		// move to the next tile spot
		x += TILE_WIDTH;
		// if reach the end of the level width we reset x
		// and move to the next row, i.e. incrementing y
		if(x >= LEVEL_WIDTH){
			x = 0;
			y += TILE_HEIGHT;
			printf("\n");
		}	
	}

	// Clip the style sheets
	tileClips[TILE_RED].x = 0;
	tileClips[TILE_RED].y = 0;
	tileClips[TILE_RED].w = TILE_WIDTH;
	tileClips[TILE_RED].h = TILE_HEIGHT;

	tileClips[TILE_GREEN].x = 0;
	tileClips[TILE_GREEN].y = 80;
	tileClips[TILE_GREEN].w = TILE_WIDTH;
	tileClips[TILE_GREEN].h = TILE_HEIGHT;

	tileClips[TILE_BLUE].x = 0;
	tileClips[TILE_BLUE].y = 160;
	tileClips[TILE_BLUE].w = TILE_WIDTH;
	tileClips[TILE_BLUE].h = TILE_HEIGHT;

	tileClips[TILE_TOPLEFT].x = 80;
	tileClips[TILE_TOPLEFT].y = 0;
	tileClips[TILE_TOPLEFT].w = TILE_WIDTH;
	tileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

	tileClips[TILE_LEFT].x = 80;
	tileClips[TILE_LEFT].y = 80;
	tileClips[TILE_LEFT].w = TILE_WIDTH;
	tileClips[TILE_LEFT].h = TILE_HEIGHT;

	tileClips[TILE_BOTTOMLEFT].x = 80;
	tileClips[TILE_BOTTOMLEFT].y = 160;
	tileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
	tileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

	tileClips[TILE_TOP].x = 160;
	tileClips[TILE_TOP].y = 0;
	tileClips[TILE_TOP].w = TILE_WIDTH;
	tileClips[TILE_TOP].h = TILE_HEIGHT;

	tileClips[TILE_CENTER].x = 160;
	tileClips[TILE_CENTER].y = 80;
	tileClips[TILE_CENTER].w = TILE_WIDTH;
	tileClips[TILE_CENTER].h = TILE_HEIGHT;

	tileClips[TILE_BOTTOM].x = 160;
	tileClips[TILE_BOTTOM].y = 160;
	tileClips[TILE_BOTTOM].w = TILE_WIDTH;
	tileClips[TILE_BOTTOM].h = TILE_HEIGHT;

	tileClips[TILE_TOPRIGHT].x = 240;
	tileClips[TILE_TOPRIGHT].y = 0;
	tileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
	tileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

	tileClips[TILE_RIGHT].x = 240;
	tileClips[TILE_RIGHT].y = 80;
	tileClips[TILE_RIGHT].w = TILE_WIDTH;
	tileClips[TILE_RIGHT].h = TILE_HEIGHT;

	tileClips[TILE_BOTTOMRIGHT].x = 240;
	tileClips[TILE_BOTTOMRIGHT].y = 160;
	tileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
	tileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;

	// close the file pointer
	fclose(map);

	return true;
error:
	return false;
}

bool loadMedia(Tile* tiles[])
{
	// load the dot texture
	bool r = Texture_loadFromFile(gWindow->renderer, &dotTexture, "Assets/dot.bmp");
	check(r != false, "ERROR : Failed to load the dot texture!");
	// load the tile texture
	r = Texture_loadFromFile(gWindow->renderer, &tileTexture, "Assets/tiles.png");
	check(r != false, "ERROR : Failed to load the tile texture!");
	// set up the tiles in the scene
	r = setTiles(tiles);
	check(r != false, "ERROR : Failed to load the tiles!");

	
	return true;
error:
	return false;
}

void handleEvents()
{
	SDL_Event e; // Queue to store the events
	
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}
		
		// Handle all the window events
		Window_handleEvents(gWindow, &e);
		
		// Handle all the dot events
		Dot_handleEvents(dot, &e);
	}	
}

void update()
{
	// Move the dot
	Dot_move(dot, tiles, LEVEL_WIDTH, LEVEL_HEIGHT);
	// Update the camera position after the dot moves
	Dot_setCamera(dot, &camera, SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT);
}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window* window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// render tiles
		for(int i = 0; i < TOTAL_TILES; i++){
			Tile_render(window->renderer, &tileTexture, tiles[i], &camera);
		}
		

		// Then we render the dot
		Dot_render(window->renderer, &camera, &dotTexture, dot);

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(gWindow);
}

void close(Tile* tiles[])
{
	
}
