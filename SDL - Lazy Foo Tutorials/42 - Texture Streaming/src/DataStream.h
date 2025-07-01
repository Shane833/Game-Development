#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <bstrlib.h>
#include <dbg.h>

typedef struct{
	SDL_Surface* images[4];
	int current_image;
	int delay_frames;
}DataStream;

DataStream* DataStream_create(); // initializes internals
bool DataStream_loadMedia(DataStream * stream); // loads the initial data
void* DataStream_getBuffer(DataStream * stream); // gets the current frame data
void DataStream_destroy(DataStream * stream); // deallocates the stream

#endif