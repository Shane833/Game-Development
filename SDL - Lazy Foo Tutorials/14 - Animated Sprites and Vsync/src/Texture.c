#include <Texture.h>

// Texture Functions 
// Initializes the variables to their equivalent zeroes
Texture* Texture_create()
{
	Texture* result = calloc(1, sizeof(Texture));
	check(result != NULL, "Failed to create a custom Texture");
	
	result->texture = NULL;
	result->width = 0;
	result->height = 0;
	
	return result;
	error:
		return NULL;
}

bool Texture_loadFromFile(SDL_Renderer* renderer, Texture* texture, const char* filepath)
{
	// First we deallocate the texture if there is already one loaded in it
	// Texture_destroy(texture);
	
	// The final texture
	SDL_Texture* new_texture = NULL;
	
	// Load the image at the specified path
	SDL_Surface* loaded_surface = IMG_Load(filepath);
	check(loaded_surface != NULL, "Failed to load the surface from %s! IMG_Error : %s\n", filepath, IMG_GetError());
	
	/* Remove in case of PNGs with Transparency
	// Color keying the image
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 255, 255));
	// Here we first provide the surface we want to color key, the we let SDL know that we want to enable
	// color key and the last is the pixel we want to color key with
	// Also the most cross platform way to create a pixel from the RGB is by using the SDL_MapRGB() function
	// First we provide the format and then the corresponding RGB values
	*/

	// Now to create a texture from surface pixels
	new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	check(new_texture != NULL, "Failed to convert surface of %s to a texture! SDL_Error : %s\n", filepath, SDL_GetError());
	
	// Get the image dimension
	texture->width = loaded_surface->w;
	texture->height = loaded_surface->h;
	
	// Get rid of the old surface
	SDL_FreeSurface(loaded_surface);
	
	texture->texture = new_texture;
	check(texture->texture != NULL, "Failed to copy the new texture of %s! SDL_Error: %s\n", filepath, SDL_GetError());
	
	return true;
	error:
		return false;
}

void Texture_render(SDL_Renderer* renderer, Texture* texture, int x, int y, SDL_Rect* clip)
{
	// Here we will define the position as to where we want to display our texture
	// First create a SDL_Rect for defining the dimension and position
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	
	check_debug(clip != NULL, "The provided clip is NULL! Rendering without it");
	
	// We only provide the area we want to render using the clip 
	// and we give that info to the render_quad
	render_quad.w = clip->w; 
	render_quad.h = clip->h;
	
	SDL_RenderCopy(renderer, texture->texture, clip, &render_quad); // Here the new argumet is the source rect which must be provided
	return;
	// Don't forget to add the return before this as it will execute as a normal flow of the program
	error:
		SDL_RenderCopy(renderer, texture->texture, NULL, &render_quad); // This is in the case the clip is not provided 
}

// Function to set the color / modulate the color
void Texture_setColor(Texture* texture, Uint8 red, Uint8 green, Uint8 blue)
{
	// We just have to make a call to an existing SDL function
	// We must provide the texture as well as the color values
	
	SDL_SetTextureColorMod(texture->texture, red, green, blue);
}

// Function to enable/disable blending on a texture
void Texture_setBlendMode(Texture* texture, SDL_BlendMode blending)
{
	// This function controls how the texture will be blended
	// There are different types of blendings which can be enabled on a texture
	// We will use SDL function SDL_SetTextureBlendMode(SDL_Texture*, SDL_BlendMode)
	SDL_SetTextureBlendMode(texture->texture, blending);
}

// Function to set the alpha value on the texture
void Texture_setAlpha(Texture* texture, Uint8 alpha)
{
	// This function will control the transparency of the texture
	// We will use the SDL function SDL_SetTextureAlphaMod(SDL_Texture*, Uint8)
	SDL_SetTextureAlphaMod(texture->texture, alpha);
}

size_t Texture_getWidth(Texture* texture)
{
	return texture->width;
}

size_t Texture_getHeight(Texture* texture)
{
	return texture->height;
}


void Texture_destroy(Texture* texture)
{
	if(texture){
		// Only getting rid of the SDL_Texture
		// SDL_DestroyTexture(texture->texture);
		texture->texture = NULL;
		texture->width = 0;
		texture->height = 0;
	}
}
