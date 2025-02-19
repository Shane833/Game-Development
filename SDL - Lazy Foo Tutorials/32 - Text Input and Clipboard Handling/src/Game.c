#include <SDL2/SDL_mixer.h> // Makes playing audio in the game easy
#include <Game.h>
#include <Texture.h>
#include <bstrlib.h>

// Some globals
// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
TTF_Font* font = NULL;
bool quit = false;

// Texture Variable
Texture input_text_texture;
Texture prompt_text_texture;
// Text Color
SDL_Color text_color = {0,0,0,255};
// Input text string
bstring input_text;
// flag for keeping check when to update the texture
bool render_text;


int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	while(!quit)
	{	
		// We set the render text flag to false initially
		render_text = false;
		
		handleEvents();
		update();
		render();	
	}
	
	close();
	
	return 0;
error:
	return 1;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	check(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0, "Failed to initialize SDL_mixer! Mix_Error: %s", Mix_GetError());
	
	window = SDL_CreateWindow("Advanced Timers",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Initializing the string
	input_text = bfromcstr("Some Text");
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	font = TTF_OpenFont("Assets/Lazy.ttf", 28);
	check(font != NULL, "Failed to load the Font!, TTF_ERROR : %s", TTF_GetError());
	
	Texture_loadFromRenderedText(renderer, &input_text_texture, font, bdata(input_text), text_color);
	Texture_loadFromRenderedText(renderer, &prompt_text_texture, font, "Enter Text:", text_color);
	
	// Enable text input
	SDL_StartTextInput();
	
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
		// Handling the Special Key Inputs
		else if (e.type == SDL_KEYDOWN){
			// Handle backspace
			if(e.key.keysym.sym == SDLK_BACKSPACE && blength(input_text) > 0){
				// Trim/lop the right most character
				bdelete(input_text, blength(input_text) - 1, 1);
				// We want to render this change
				render_text = true;
			}
			// Handling Copy
			else if(e.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)){
				// Copying the text into clipboard with this function
				SDL_SetClipboardText(bdata(input_text));
			}
			// Handle pasting
			else if(e.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL)){
				// Copy the text from the temporary buffer
				char* temp_text = SDL_GetClipboardText();
				input_text = bfromcstr(temp_text);
				SDL_free(temp_text); // destroy the temporary buffer
				// Using SDL_free as it was allocated using the SDL memory allocator
				
				// and finally we wish to display the changes
				render_text = true;
			}
		}
		// Special text input event
		else if (e.type == SDL_TEXTINPUT){
			// Not copying or pasting just typing data
			if(!((SDL_GetModState() & KMOD_CTRL) && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))){
				// Then we will simply append the character onto the string
				// don't know if e.text.text is a character or not check it 
				// I found out its not a single character but a whole string (char*)
				// So lets convert it from a c string to bstr
				const_bstring temp = bfromcstr(e.text.text);
				bconcat(input_text, temp);
				bdestroy(temp);
				render_text = true;
			}
		}
		
	}	
}

void update()
{
	// Render the text if needed
	if(render_text){
		// If text is not empty
		if(blength(input_text) != 0){
			// Create new text from the string
			Texture_loadFromRenderedText(renderer, &input_text_texture, font, bdata(input_text), text_color);
		}
		// and if the text is empty
		else{
			// Render Space Texture / Empty
			Texture_loadFromRenderedText(renderer, &input_text_texture, font, " ", text_color);
		}
	}
}

void render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Render text textures
	Texture_render(renderer, &prompt_text_texture, (SCREEN_WIDTH - Texture_getWidth(&prompt_text_texture)) / 2, 0, NULL);
	Texture_render(renderer, &input_text_texture, (SCREEN_WIDTH - Texture_getWidth(&input_text_texture)) / 2, Texture_getHeight(&prompt_text_texture), NULL);

	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	// Disable Text Input
	SDL_StopTextInput();
	
	// destroy the string
	bdestroy(input_text);
	
	
	// Close our window and the renderer
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
