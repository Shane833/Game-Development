#include <Game.h>
#include <Timer.h>
#include <Sound.h>
#include <bstrlib.h>

// Usual Global Variables
SDL_Color text_color = {0,0,0,255};
Timer * fps_timer = NULL; // timer to keep track of ms passed
bstring time_text = NULL; // used for string as a time

// Prompt texture
Texture * prompt_texture = NULL;
// The text textures that specify the recording devices names
Texture* device_textures[MAX_RECORDING_DEVICES] = { NULL };

// Number of available devices
int recording_device_count = 0;

int main(int arg, char* argv[])
{
	int r = run();
	check(r == 0, "Something went wrong!");
	
error: // close with fallthrough
	// Destroy the string
	bdestroy(time_text);
	time_text = NULL;

	// Close the timer
	Timer_destroy(fps_timer);
	fps_timer = NULL;

	// Close the texture
	Texture_destroy(prompt_texture);
	prompt_texture = NULL;

	for(int i = 0;i < MAX_RECORDING_DEVICES;i++){
		if(device_textures[i]){
			Texture_destroy(device_textures[i]);
			device_textures[i] = NULL;
		}
	}

	// Close all of our windows
	Window_destroy(window);
	window = NULL;

	// Close the font
	TTF_CloseFont(font);
	font = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return r;
}

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	// Start the timer before entering the loop
	Timer_start(fps_timer);

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


// Function definitions
bool init()
{
	// Now we initialize the audio subsystem too
	check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) <= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	
	// Here we initialize the window
	window = Window_create("Calculating Framerate", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	check(window != NULL, "ERROR : Failed to create window!");

	// Create a Texture
	prompt_texture = Texture_create();
	check(prompt_texture != NULL, "ERROR : Failed to create Texture!");

	// Create our timer
	fps_timer = Timer_create();
	check(fps_timer != NULL, "ERROR : Failed to create the timer!");

	// Create our bstring
	time_text = bfromcstr("Average FPS : ");
	check(time_text != NULL, "ERROR : Failed to create the bstring!");
	
	return true;
error:
	return false;
}


bool loadMedia()
{
	// load the font from the file
	font = TTF_OpenFont("Assets/lazy.ttf", 28);
	check(font != NULL, "ERROR : Failed to load the font!, TTF Error : %s", TTF_GetError());
	
	// Load up the texture with prompt
	Texture_loadFromRenderedText(prompt_texture, window, font, "Select recording device : ", text_color);

	// Get the device capture count, fetching the available recording devices
	// With SDL_TRUE argument, we get the no. of recording devices
	// With SDL_FALSE argument, we get the no. of playback devices
	recording_device_count = SDL_GetNumAudioDevices( SDL_TRUE );
	check(recording_device_count > 0, "ERROR : Unable to get audio capture device! SDL Error : %s", SDL_GetError());

	// If atlease one device is connected 
	// Cap the recording device count
	if(recording_device_count > MAX_RECORDING_DEVICES){
		recording_device_count = MAX_RECORDING_DEVICES;
	}

	// Then we fetch the names of these devices and display them
	// to be selected by the user
	for(int i = 0;i < recording_device_count;i++){
		char ibuff[2]; // stores the string version of i
		snprintf(ibuff, sizeof(ibuff), "%d", i);

		// bstring for concatenation
		bstring prompt_text = bfromcstr(ibuff);
		check(prompt_text != NULL, "ERROR : Failed to create bstring!");

		// concatenate data to the text
		bcatcstr(prompt_text, " : ");
		bcatcstr(prompt_text, SDL_GetAudioDeviceName(i, SDL_TRUE));

		// Create a text texture from this
		device_textures[i] = Texture_create();
		check(device_textures[i] != NULL, "ERROR : Failed to create the texture!");

		// load up the texture with the font
		Texture_loadFromRenderedText(device_textures[i], window, font, bdata(prompt_text), text_color);
		
		// deallocate the bstring after its use
		bdestroy(prompt_text);
	}

	

	return true;
error:
	return false;
}

void handleEvents()
{	
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}

		// Do current state event handling
		switch(current_state){
			// User is selecting recording devices
			case SELECTING_DEVICE:
				// on key press
				if(e.type == SDL_KEYDOWN){
					// Handle Key Press form 0 to 9
					if(e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9){
						// Get selection index
						int index = e.key.keysym.sym - SDLK_0;
						// check the validity of the index
						if(index < recording_device_count){
							// Default audio spec
							SDL_AudioSpec desired_recording_spec;
							SDL_zero(desired_recording_spec);
							desired_recording_spec.freq = 44100;
							desired_recording_spec.format = AUDIO_F32;
							desired_recording_spec.channels = 2;
							desired_recording_spec.samples = 4096;
							desired_recording_spec.callback = Sound_audioRecordingCallback;

							// Open the recording device, here the 2nd arg SDL_TRUE suggests we want to open a recording device
							recording_device_ID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, SDL_TRUE), SDL_TRUE, &desired_recording_spec, &received_recording_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);	
							
							// Device failed to open
							if(recording_device_ID == 0){
								log_err("Failed to open  recording device! SDL Error : %s", SDL_GetError());
								// Update the text on the prompt texture
								Texture_loadFromRenderedText(prompt_texture, window, font, "Failed to open recording device!", text_color);
								current_state = ERROR;
							}else{ 
								// Device opened successfully
								// Now we create a spec for playback device
								SDL_AudioSpec desired_playback_spec;
								SDL_zero(desired_playback_spec);
								desired_playback_spec.freq = 44100;
								desired_playback_spec.format = AUDIO_F32;
								desired_playback_spec.channels = 2;
								desired_playback_spec.samples = 4096;
								desired_playback_spec.callback = Sound_audioPlaybackCallback;

								// Open playback device
								playback_device_ID = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desired_playback_spec, &received_playback_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
								
								// If device failed to open
								if(playback_device_ID == 0){
									// Report Error
									log_err("Failed to open playback device! SDL Error : %s", SDL_GetError());
									Texture_loadFromRenderedText(prompt_texture, window, font, "Failed to open playback device!", text_color);
									current_state = ERROR;
								}else{
									// Device opened successfully

									// Calculate per sample bytes
									int bytes_per_sample = received_recording_spec.channels * (SDL_AUDIO_BITSIZE(received_recording_spec.format) / 8 );

									// Calculate bytes per second
									int bytes_per_second = received_recording_spec.freq * bytes_per_sample;

									// calculate buffer size
									buffer_byte_size = RECORDING_BUFFER_SECONDS * bytes_per_second;

									// calculate max buffer use
									buffer_byte_max_position = MAX_RECORDING_SECONDS * bytes_per_second;

									// Allocate and initialize byte buffer
									recording_buffer = calloc(buffer_byte_size, sizeof(Uint8));
									// then initialize the memory with all zeroes
									memset(recording_buffer, 0, buffer_byte_size);

									// go on to the next state
									Texture_loadFromRenderedText(prompt_texture, window, font, "Press 1 to record for 5 seconds.", text_color);
									current_state = STOPPED;
								}
							}
						}
					}
				}
				break;
		
		}
		
		// Handle all the window events
		Window_handleEvents(window, &e);
	
	}

}

void update()
{
	

	

error:
	return;
}

// We declared this function to be extern in the Window.h file
// That means its defintion will be defined somewhere else possible other files
void Window_render(Window * window)
{
	// Again we only want to draw to a window if its not minimized
	if(!window->minimized){
		
		// clear screen
		SDL_SetRenderDrawColor(window->renderer, 255,255,255,255);
		SDL_RenderClear(window->renderer);
		
		// render the prompt on the screen
		Texture_render(prompt_texture, window, (SCREEN_WIDTH - Texture_getWidth(prompt_texture)) / 2, Texture_getHeight(prompt_texture), NULL);

		int y = Texture_getHeight(prompt_texture); // just muliples of this
		// Render the various device names based on the state
		for(int i = 0;i < recording_device_count;i++){
			Texture_render(device_textures[i], window, 0, y * (i + 2), NULL);
		}

		// update screen
		SDL_RenderPresent(window->renderer);
		
	}
}

void render()
{
	// Render the window
	Window_render(window);
}

