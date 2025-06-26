#include <Texture.h>

// Texture Functions 
// Initializes the variables to their equivalent zeroes
Texture * Texture_create()
{
	Texture * result = calloc(1, sizeof(Texture));
	check(result != NULL, "Failed to create a custom Texture");
		
	return result;
error:
	return NULL;
}

void Texture_destroy(Texture * texture)
{
	check(texture != NULL, "ERROR : Invalid Texture!");
	// Only getting rid of the SDL_Texture
	SDL_DestroyTexture(texture->texture);
	texture->texture = NULL;
	texture->width = 0;
	texture->height = 0;
	
	debug(texture->pixels_surface != NULL, "ERROR : Invalid Pixels Surface!");
	SDL_FreeSurface(texture->pixels_surface);
	texture->pixels_surface = NULL;

error:
	return;
}

bool Texture_loadFromFile(Window * window, Texture * texture, const char * filepath)
{
	// First we deallocate the texture if there is already one loaded in it
	Texture_destroy(texture);
	
	// The final texture
	SDL_Texture * new_texture = NULL;
	
	// Load the image at the specified path
	SDL_Surface * loaded_surface = IMG_Load(filepath);
	check(loaded_surface != NULL, "Failed to load the surface from %s! IMG_Error : %s", filepath, IMG_GetError());
	
	// Color keying the image
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 255, 255));
	// Here we first provide the surface we want to color key, the we let SDL know that we want to enable
	// color key and the last is the pixel we want to color key with
	// Also the most cross platform way to create a pixel from the RGB is by using the SDL_MapRGB() function
	// First we provide the format and then the corresponding RGB values
	
	// Now to create a texture from surface pixels
	new_texture = SDL_CreateTextureFromSurface(window->renderer, loaded_surface);
	check(new_texture != NULL, "Failed to convert surface of %s to a texture! SDL_Error : %s", filepath, SDL_GetError());
	
	// Get the image dimension
	texture->width = loaded_surface->w;
	texture->height = loaded_surface->h;
	
	// Get rid of the old surface
	SDL_FreeSurface(loaded_surface);
	
	texture->texture = new_texture;
	check(texture->texture != NULL, "Failed to copy the new texture of %s! SDL_Error: %s", filepath, SDL_GetError());
	
	return true;
error:
	return false;
}

bool Texture_loadPixelsFromFile(Window * window, Texture * texture, const char * filepath)
{
	// Free pre-existing assets
	Texture_destroy(texture);
	
	// Load the image at specified path
	SDL_Surface * loaded_surface = IMG_Load(filepath);
	check(loaded_surface != NULL, "ERROR : Failed to load the Image File!, SDL Error : %s", SDL_GetError());
	
	// Conver the surface to display format
	texture->pixels_surface = SDL_ConvertSurfaceFormat(loaded_surface, SDL_GetWindowPixelFormat(window->window), 0);
	check(texture->pixels_surface != NULL, "ERROR : Failed to convert the loaded texture to display format!, SDL Error : %s", SDL_GetError());
	
	// Get Image Dimensions
	texture->width = texture->pixels_surface->w;
	texture->height = texture->pixels_surface->h;
	
	// Get rid of the old loaded surface 
	SDL_FreeSurface(loaded_surface);
	
	return true;
error: 
	return false;
}

bool Texture_loadFromPixels(Window * window, Texture * texture)
{
	// only load if pixels exists
	check(texture->pixels_surface != NULL, "ERROR : No Pixels Loaded!, SDL Error : %s", SDL_GetError());
	
	// Color key the image
	SDL_SetColorKey(texture->pixels_surface, SDL_TRUE, SDL_MapRGB(texture->pixels_surface->format, 0, 255, 255));
	
	// Create texture from surface pixels
	texture->texture = SDL_CreateTextureFromSurface(window->renderer, texture->pixels_surface);
	check(texture != NULL, "ERROR : Failed to create texture from surface pixels!, SDL Error : %s", SDL_GetError());
	
	// Get Image Dimensions
	texture->width = texture->pixels_surface->w;
	texture->height = texture->pixels_surface->h;
	
	// Get rid of old loaded pixels
	SDL_FreeSurface(texture->pixels_surface);
	
	return true;
error: 
	return false;
}

void Texture_render(Window * window, Texture * texture, int x, int y, SDL_Rect * clip)
{	
	// Here we will define the position as to where we want to display our texture
	// First create a SDL_Rect for defining the dimension and position
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	
	check_debug(clip != NULL, "The provided clip is NULL! Rendering without it");
	
	// We only provide the area we want to render using the clip 
	// and we give that info to the render_quad
	render_quad.w = clip->w; 
	render_quad.h = clip->h;
	
	SDL_RenderCopy(window->renderer, texture->texture, clip, &render_quad); // Here the new argumet is the source rect which must be provided
	return;
	// Don't forget to add the return before this as it will execute as a normal flow of the program
error:
	SDL_RenderCopy(window->renderer, texture->texture, NULL, &render_quad); // This is in the case the clip is not provided 
}

void Texture_renderEx(Window * window, Texture * texture, int x, int y, SDL_Rect * clip, double angle, SDL_Point * center, SDL_RendererFlip flip)
{
	// Added more functionality :
	// now the function takes 3 additional arguments that are:
	// 1. The angle of rotation
	// 2. The point around which the texture must rotate
	// 4. A SDL_RendererFlip enum
	
	// The new render function we call is SDL_RenderEx() which accepts these additional arguments
	
	// Here we will define the position as to where we want to display our texture
	// First create a SDL_Rect for defining the dimension and position
	SDL_Rect render_quad = {x, y, texture->width, texture->height};
	
	check_debug(clip != NULL, "The provided clip is NULL! Rendering without it");
	
	// We only provide the area we want to render using the clip 
	// and we give that info to the render_quad
	render_quad.w = clip->w; 
	render_quad.h = clip->h;
	
	SDL_RenderCopyEx(window->renderer, texture->texture, clip, &render_quad, angle, center, flip);
	return;
	// Don't forget to add the return before this as it will execute as a normal flow of the program
error:
	SDL_RenderCopyEx(window->renderer, texture->texture, NULL, &render_quad, angle, center, flip); // This is in the case the clip is not provided 
}
 

// Function to set the color / modulate the color
void Texture_setColor(Texture * texture, Uint8 red, Uint8 green, Uint8 blue)
{
	// We just have to make a call to an existing SDL function
	// We must provide the texture as well as the color values
	
	SDL_SetTextureColorMod(texture->texture, red, green, blue);
}

// Function to enable/disable blending on a texture
void Texture_setBlendMode(Texture * texture, SDL_BlendMode blending)
{
	// This function controls how the texture will be blended
	// There are different types of blendings which can be enabled on a texture
	// We will use SDL function SDL_SetTextureBlendMode(SDL_Texture *, SDL_BlendMode)
	SDL_SetTextureBlendMode(texture->texture, blending);
}

// Function to set the alpha value on the texture
void Texture_setAlpha(Texture * texture, Uint8 alpha)
{
	// This function will control the transparency of the texture
	// We will use the SDL function SDL_SetTextureAlphaMod(SDL_Texture *, Uint8)
	SDL_SetTextureAlphaMod(texture->texture, alpha);
}

size_t Texture_getWidth(Texture * texture)
{
	return texture->width;
}

size_t Texture_getHeight(Texture * texture)
{
	return texture->height;
}

bool Texture_loadFromRenderedText(Window * window, Texture * texture, TTF_Font * font, const char * text, SDL_Color text_color)
{
	// Get rid of the preexisting texture
	Texture_destroy(texture);
	
	// Render the text surface
	// For rendering text use the TTF_RenderText_ *(TTF_Font *, text, SDL_Color)
	// This function returns a SDL_Surface on successful render of the text
	SDL_Surface * temp_text = TTF_RenderText_Solid(font, text, text_color); 
	check(temp_text != NULL, "Unable to render text surface! SDL_ttf Error: %s", TTF_GetError());
	
	// Now create a texture from the surface as usual
	texture->texture = SDL_CreateTextureFromSurface(window->renderer, temp_text);
	check(texture->texture != NULL, "Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());
	
	// Now get the image dimensions
	texture->width = temp_text->w;
	texture->height = temp_text->h;
	
	// Get rid of the old surface
	SDL_FreeSurface(temp_text);
	
	return true;
error:
	return false;
}

Uint32 * Texture_getPixels32(Texture * texture)
{
	check(texture != NULL, "ERROR : Invalid Texture!");
	check(texture->pixels_surface != NULL, "ERROR : Invalid Acess of Pixels!");
	
	Uint32 * pixels = NULL;
	
	pixels = texture->pixels_surface->pixels;
	
	return pixels;
error:
	return NULL;
}

Uint32 Texture_getPixel32(Texture * texture, Uint32 x, Uint32 y)
{
	check(texture != NULL, "ERROR : Invalid Texture!");
	
	Uint32 * pixels = (Uint32 * )texture->pixels_surface->pixels; // convert the pixels to 32bits
	
	return pixels[ (y * Texture_getPitch32(texture)) + x ]; // return the requested pixel
	// This is equivalent of addressing modes in arrays rows are depicted by y and then position
	// of inidividual columns are given by x 
error:
	return 0;
}

Uint32 Texture_getPitch32(Texture * texture)
{
	check(texture != NULL, "ERROR : Invalid Texture!");
	check(texture->pixels_surface != NULL, "ERROR : Invalid Access of Pixels!");
	
	// Pitch kind of acts like width, depicting how much memory in bits
	// it takes for the horizontal component (hence the width)
	// When we divide the total bits by 4 hence acquiring the no. of bytes
	// it turns out to be equivalent to the no. of pixels in the texture horizontally
	
	Uint32 pitch = 0;
	
	pitch = texture->pixels_surface->pitch / 4;
	
	return pitch;
error:
	return 0;
}

// This function simply returns a 32bit pixel in the internal format
Uint32 Texture_mapRGBA(Texture * texture, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	check(texture != NULL, "ERROR : Invalid Texture!");
	check(texture->pixels_surface != NULL, "ERROR : Invalid Access of Pixels!");
	
	Uint32 pixel = 0;
	
	pixel = SDL_MapRGBA(texture->pixels_surface->format, red, green, blue, alpha);
	
	return pixel;
error:
	return 0;
}