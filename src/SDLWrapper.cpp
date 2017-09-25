/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @SDLWrapper.cpp
* Handles player input. Using SDL event handling, recieves the player input accordingly.
*/

#include "SDLWrapper.h"
#include "Logger.h"

/**
* The system initializer.
* Initializes all the SDL systems and/or subsystems.
* @return True if every system was initialized successfully, else it returns false.
*/
bool SDLWrapper::initialize()
{
	bool successSDL = false;
	bool successIMG = false;
	bool successMixer = false;
	bool successTTF = false;

	SDL_version compiled;

	Log(DEBUG) << "Initializing systems...";

	// Initializing SDL_TTF.
	const int ttfInit = TTF_Init();
	if ( ttfInit == 0 )
  {
		successTTF = true;

		SDL_TTF_VERSION(&compiled);
		SDLWrapper::logSDLVersion( "SDL_TTF", compiled );
	}

	else
  {
		Log(ERROR) << "Could not initialize TTF." << TTF_GetError();
	}

	// Initializing SDL with initFlags.
	const Uint32 initFlags = SDL_INIT_EVERYTHING;
	const int sdlInit = SDL_Init( initFlags );

	if ( sdlInit == 0 )
  {
		successSDL = true;

		SDL_version linked;
		SDL_VERSION( &compiled );
		SDL_GetVersion( &linked );

		SDLWrapper::logSDLVersion( "SDL", compiled, SDL_GetRevision() );
	}

	else
  {
		Log(ERROR) << "Could not initialize SDL." << SDL_GetError();
	}

	// Initializing SDL_image with imgFlags.
	const Uint32 imgFlags = IMG_INIT_PNG;
	if ( (IMG_Init( imgFlags) & imgFlags ) )
  {
		successIMG = true;

		SDL_IMAGE_VERSION( &compiled );
		SDLWrapper::logSDLVersion( "SDL_image", compiled );
	}

	else
  {
		Log(ERROR) << "Could not initialize SDL_Image." << IMG_GetError();
	}

	// Initializing SDL_mixer.
	const int frequency = 44100;
	const int channels = 2;
	const int chunksize = 4096;
	const int initialized = Mix_OpenAudio( frequency, MIX_DEFAULT_FORMAT, channels, chunksize );
	
  if ( initialized == 0 )
  {
		successMixer = true;

		SDL_MIXER_VERSION( &compiled );
		SDLWrapper::logSDLVersion( "SDL_mixer", compiled );
	}

	else
  {
		Log(ERROR) << "Could not initialize SDL_Mixer" << Mix_GetError();
	}

	// If even one system fails to initialize, returns false.
	return ( successSDL && successIMG && successMixer && successTTF );
}

/**
* Closes all open systems.
* Closes all systems and/or subsystems opened by the initializer method.
* @see initialize()
*/
void SDLWrapper::close ()
{
	Log(DEBUG) << "Closing SDL.";

	// Quits SDL_mixer.
	Mix_CloseAudio();
	Mix_Quit();

	// Quits SDL_image.
	IMG_Quit();

	// Quits SDL.
	SDL_Quit();

	// Quits SDL_TTF.
	TTF_Quit();
}

/**
* Logs the SDL API version.
* @param what_ : What API is being logged about.
* @param compiled_ : The compiled version.
* @param linked_ : The linked version.
* @param revision_ : If any, the revision.
*/
void SDLWrapper::logSDLVersion ( const std::string& what_, const SDL_version& compiled_,
	std::string revision_ )
{

	Log(DEBUG) << what_ << " Version (Compiled): " << ( int )compiled_.major << "." <<
		( int )compiled_.minor << "." << ( int )compiled_.patch <<
		( ( !revision_.empty() ) ? revision_ : "" );
}
