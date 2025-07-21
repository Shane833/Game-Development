#include <Sound.h>

// Recording data buffer
Uint8 * recording_buffer = NULL;

Uint32 buffer_byte_size = 0;

Uint32 buffer_byte_position = 0;

Uint32 buffer_byte_max_position = 0;

void Sound_audioRecordingCallback(void * user_data, Uint8 * stream, size_t len)
{

}

void Sound_audioPlaybackCallback(void * user_data, Uint8 * stream, size_t len)
{

}