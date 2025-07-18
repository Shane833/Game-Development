#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <Texture.h>

// Maximum no. of supported recording devices
constexpr int MAX_RECORDING_DEVICES = 10;

// Maximum recording time
constexpr int MAX_RECORDING_SECONDS = 5;

// Maximum recording time plus padding
constexpr int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

// Various recording actions we can take

enum{
	SELECTING_DEVICE,
	STOPPED,
	RECORDING,
	RECORDED,
	PLAYBACK,
	ERROR
};

// Recording/Playback callbacks
// These are the functions that will actually record to and play from our audio buffer
void Sound_audioRecordingCallback(void * user_data, Uint8 * stream, size_t len);
void Sound_audioPlaybackCallback(void * user_data, Uint8 * stream, size_t len);

// Received audio spec
// This basically defines how the audio will be recorded/playback on this device
// This specification is returned back by the audio driver and we will store it
SDL_AudioSpec received_recording_spec;
SDL_AudioSpec received_playback_spec;

// Recording data buffer
Uint8 * recording_buffer = NULL;
// Size of data buffer, defines how much the buffer will store
// This will be equivalent of 6 seconds of bytes (5 + 1)
Uint32 buffer_byte_size = 0;
// Position in data buffer
Uint32 buffer_byte_position = 0;
// Maximum position in data buffer for recording
// This is the actual 5 seconds of bytes we will be using
Uint32 buffer_byte_max_position = 0;




#endif