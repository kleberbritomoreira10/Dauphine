#include "Sprite.h"
#include "Window.h"
#include "Logger.h"
#include <cassert>

Sprite::Sprite(const std::string& PATH) :
	sdlTexture(nullptr),
	width(0),
	height(0),
	path(PATH),
	flipHorizontal(false)
{
	loadFrom(this->path);

	setBlendMode(SDL_BLENDMODE_BLEND);
	setAlpha(255);
}

Sprite::Sprite(SDL_Surface* const surface_) :
	sdlTexture(surfaceToTexture(surface_)),
	width(0),
	height(0),
	path("font"),
	flipHorizontal(false)
{
	// Display error log if image wasn't loaded.
	if(this->sdlTexture == nullptr){
		Log(ERROR) << "Sprite load failed: " << this->path;
	}
}

Sprite::~Sprite(){
	if(this->sdlTexture != nullptr){
		SDL_DestroyTexture(this->sdlTexture);
		this->sdlTexture = nullptr;
	}
}

void Sprite::loadFrom(const std::string& PATH){
	assert(Window::get_renderer() != nullptr && "Window renderer should not be null!");


	SDL_Surface* loadedSurface = IMG_Load(PATH.c_str());

	// Returns whether the Sprites texture is null or not.
	this->sdlTexture = surfaceToTexture(loadedSurface);

	// Display error log if image wasn't loaded.
	if(this->sdlTexture == nullptr){
		Log(ERROR) << "Sprite load failed: " << PATH;
	}
}

void Sprite::render(const double x_, const double y_, SDL_Rect* const clip_,
	const bool stretch_, const double angle_, SDL_Point* const center_,
	SDL_RendererFlip flip_){

	// This is the destination SDL_Rect structure.
	SDL_Rect renderQuad = {(int)x_, (int)y_, (int)this->width, (int)this->height};

	if(clip_ != nullptr){
		renderQuad.w = clip_->w;
		renderQuad.h = clip_->h;
	}
	else{
		// Don't clip the sprite.
	}

	int logicalW = 0;
	int logicalH = 0;
	Window::get_logical_size(&logicalW, &logicalH);
	SDL_Rect stretch = {(int)x_, (int)y_, logicalW, logicalH};

	const int successfullRender = (!stretch_) ?
		SDL_RenderCopyEx(Window::get_renderer(), this->sdlTexture, clip_,
			&renderQuad, angle_, center_, flip_) :
		SDL_RenderCopyEx(Window::get_renderer(), this->sdlTexture, clip_,
			&stretch, angle_, center_, flip_);
	
	if(successfullRender != 0){
		Log(ERROR) << "Failed to render sprite." << SDL_GetError();
	}
}

unsigned int Sprite::getWidth(){
	return this->width;
}

unsigned int Sprite::getHeight(){
	return this->height;
}

void Sprite::setWidth(unsigned int width_){
	this->width = width_;
}

void Sprite::setHeight(unsigned int height_){
	this->height = height_;
}

SDL_Texture* Sprite::surfaceToTexture(SDL_Surface* const surface_){
	// The final texture.
	SDL_Texture* newTexture = nullptr;

	if(surface_ != nullptr){
		// Create texture from the surface pixels.
        newTexture = SDL_CreateTextureFromSurface(Window::get_renderer(), surface_);
		if(newTexture != nullptr){
			// Set the Sprites width and height, from the loaded surface.
			this->width = surface_->w;
			this->height = surface_->h;
		}
		else{
			Log(ERROR) << "Could not create texture from surface." << SDL_GetError();
		}

		// Free the loaded surface.
		SDL_FreeSurface(surface_);
	}
	else{
		Log(ERROR) << "Could not load surface from path." << IMG_GetError();
	}

	return newTexture;
}

std::string Sprite::getPath(){
	return this->path;
}

double Sprite::getAlpha(){
	Uint8 alpha = 0;

	const int rc = SDL_GetTextureAlphaMod(this->sdlTexture, &alpha);
	if(rc != 0){
		Log(ERROR) << "Could not get alpha value from Sprite (" << this->path << "). " << SDL_GetError();
	}

	return ((double)alpha); // /255.0
}

void Sprite::setAlpha(int alpha_){
	if(alpha_ < 0){
		alpha_ = 0;
	}
	else if(alpha_ > 255){
		alpha_ = 255;
	}

	const int rc = SDL_SetTextureAlphaMod(this->sdlTexture, alpha_);
	if(rc != 0){
		Log(ERROR) << "Could not set alpha value of Sprite (" << this->path << "). " << SDL_GetError();
	}
}

void Sprite::setBlendMode(SDL_BlendMode blending_){
	const int rc = SDL_SetTextureBlendMode(this->sdlTexture, blending_);
	if(rc != 0){
		Log(ERROR) << "Could not set blend mode of Sprite (" << this->path << "). " << SDL_GetError();
	}
}
