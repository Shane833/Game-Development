#include <Sound.h>

// Recording data buffer
Uint8 * recording_buffer = NULL;

Uint32 buffer_byte_size = 0;

Uint32 buffer_byte_position = 0;

Uint32 buffer_byte_max_position = 0;

// Other initializations
RecordingState current_state = SELECTING_DEVICE;

SDL_AudioDeviceID recording_device_ID = 0;
SDL_AudioDeviceID playback_device_ID = 0;

// Declare Audio Specs ( This is a global to by default initialized to 0 or equivalent)
SDL_AudioSpec received_recording_spec;
SDL_AudioSpec received_playback_spec;

void Sound_audioRecordingCallback(void * user_data, Uint8 * stream, int len)
{
	// This get called at regular intervals whenever we unpause the recording device

	// Copy audio from stream
	// here we are assigning single chunk of data to each position in the buffer
	memcpy(&recording_buffer[buffer_byte_position], stream, len);

	// increment the byte position by one chunk
	buffer_byte_position += len;
}

void Sound_audioPlaybackCallback(void * user_data, Uint8 * stream, int len)
{
	// This gets called at regular intervals whenever we unpause the playback device

	// Copy audio from the buffer and put it in the stream
	memcpy(stream, &recording_buffer[buffer_byte_position], len);

	// Update the current byte position
	buffer_byte_position += len;
}