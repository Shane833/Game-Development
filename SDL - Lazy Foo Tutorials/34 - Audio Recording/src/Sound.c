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

}

void Sound_audioPlaybackCallback(void * user_data, Uint8 * stream, int len)
{

}