#include <DataStream.h>
#include <stdio.h> // for using sprintf

DataStream* DataStream_create()
{
	DataStream * temp = malloc(sizeof(DataStream));
	check(temp != NULL, "ERROR : Failed to create the DataStream!");

	temp->images[0] = NULL;
	temp->images[1] = NULL;
	temp->images[2] = NULL;
	temp->images[3] = NULL;

	temp->current_image = 0;
	temp->delay_frames = 4;

	return temp;

error:
	return NULL;
}

bool DataStream_loadMedia(DataStream * stream)
{
	check(stream != NULL, "ERROR : Invalid Stream!");

	bstring path = bfromcstr("Assets/foo_walk_");
	check(path != NULL, "ERROR : Failed to create the path!");

	// Now we are going to load the image the images one by one into the surface array
	for(int i = 0; i < 4;i++){
		// Lets generate the total path
		char num[2]; // used for converting i to a string
		sprintf(num, "%d", i);

		bstring total_path = bstrcpy(path);
		check(total_path != NULL, "ERROR : Failed to create the total path!");

		// concatenate the remaining string
		bcatcstr(total_path, num);
		bcatcstr(total_path, ".png");

		SDL_Surface * loaded_surface = IMG_Load(bdata(total_path));
		check(loaded_surface != NULL, "ERROR : Failed to load the image, SDL_image Error : %s", IMG_GetError());

		stream->images[i] = SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0);
		check(stream->images[i] != NULL, "ERROR : Failed to convert the surface format!");

		SDL_FreeSurface(loaded_surface);
		bdestroy(total_path);
	}

	bdestroy(path);

	return true;
error:
	return false;
}

void* DataStream_getBuffer(DataStream * stream)
{
	check(stream != NULL, "ERROR : Invalid Stream!");

	// simple looping through the images

	stream->delay_frames--;

	if(stream->delay_frames == 0){
		stream->current_image++;
		stream->delay_frames = 4;
	}

	if(stream->current_image == 4){
		stream->current_image = 0;
	}

	return (stream->images[stream->current_image])->pixels;
error:
	return NULL;
}

void DataStream_destroy(DataStream * stream)
{
	if(stream){
		for(int i = 0;i < 4;i++){
			if(stream->images[i]){
				SDL_FreeSurface(stream->images[i]);
				stream->images[i] = NULL;
			}
		}
		free(stream);
	}
}

