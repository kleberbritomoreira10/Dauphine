/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @SDLWrapper.cpp
* Handles player input. Using SDL event handling, recieves the player input accordingly.
*/

#include "SDLWrapper.h"
#include "Logger.h"
#include <assert.h>
#include <cstddef>

/**
* The system initializer.
* Initializes all the SDL systems and/or subsystems.
* @return True if every system was INITIALIZED successfully, else it returns false.
*/
bool SDLWrapper::initialize()
{    
	SDL_version compiled;

	Log(DEBUG) << "Initializing systems...";
	
	bool successTTF = false;
	bool successSDL = false;
	bool success_image = false;
	bool successMixer = false;

	successTTF = SDL_TTF(compiled);
	successSDL = SUCCESS_SDL(compiled); 	
	success_image = SDL_success_image(compiled);
	successMixer = SDL_mixer(compiled);

	// If even one system fails to initialize, returns false.
	return ( successSDL && success_image && successMixer && successTTF );
}

bool SDLWrapper::SDL_mixer(SDL_version compiled)
{
  // Initializing SDL_mixer.
	const int FREQUENCY = 44100;
	const int CHANNELS = 2;
	const int CHUNKSIZE = 4096;
	const int INITIALIZED = Mix_OpenAudio( FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNKSIZE );

	bool successMixer = false;  //Declaring boolean variable that checks if INITIALIZED = 0
  if ( INITIALIZED == 0 )
  {
		successMixer = true;

		SDL_MIXER_VERSION( &compiled );
		SDLWrapper::logSDLVersion( "SDL_mixer", compiled );
  } else
    {
		  Log(ERROR) << "Could not initialize SDL_Mixer" << Mix_GetError();
	  }

	if (successMixer == true)
  {
    Log(DEBUG) << "successMixer initialized with success"; 
  } else
    {
      Log(DEBUG) << "successMixer not initialized";
    }

	return successMixer;
}

bool SDLWrapper::SDL_TTF(SDL_version compiled)
{
  bool successTTF = false;  //Declaring boolean variable that checks if ttfInit = 0
	// Initializing SDL_TTF.
	const int ttfInit = TTF_Init();
	if ( ttfInit == 0 )
  {
		successTTF = true;

		SDL_TTF_VERSION(&compiled);
		SDLWrapper::logSDLVersion( "SDL_TTF", compiled );
	} else
    {
		  Log(ERROR) << "Could not initialize TTF." << TTF_GetError();
	  }

	if (successTTF == true)
  {
    Log(DEBUG) << "successTTF initialized with success"; 
  } else
    {
      Log(DEBUG) << "successTTF not initialized";
    }

	return successTTF;  
}

bool SDLWrapper::SUCCESS_SDL(SDL_version compiled)
{
	// Initializing SDL with initFlags.
	const Uint32 initFlags = SDL_INIT_EVERYTHING;
	const int sdlInit = SDL_Init( initFlags );

	bool successSDL = false;     //Declaring boolean variable that checks if sdlInit = 0
	if ( sdlInit == 0 )
  {
		successSDL = true;

		SDL_version linked;
		SDL_VERSION( &compiled );
		SDL_GetVersion( &linked );

		SDLWrapper::logSDLVersion( "SDL", compiled, SDL_GetRevision() );
	} else
    {
		  Log(ERROR) << "Could not initialize SDL." << SDL_GetError();
	  }

	if (successSDL == true)
  {
    Log(DEBUG) << "successSDL initialized with success"; 
  } else
    {
      Log(DEBUG) << "successSDL not initialized";
    }
      
	return successSDL;
}

bool SDLWrapper::SDL_success_image(SDL_version compiled)
{	
	bool success_image = false;  //Declaring boolean variable that checks if image init
	// Initializing SDL_image with image_flags.
	const Uint32 image_flags = IMG_INIT_PNG;
	if ( (IMG_Init( image_flags) & image_flags ) )
  {
		success_image = true;

		SDL_IMAGE_VERSION( &compiled );
		SDLWrapper::logSDLVersion( "SDL_image", compiled );
	} else
    {
		  Log(ERROR) << "Could not initialize SDL_Image." << IMG_GetError();
	  }

	if (success_image == true)
  {
    Log(DEBUG) << "success_image initialized with success"; 
  } else
    {
      Log(DEBUG) << "success_image not initialized";
    } 
	 
	return success_image; 
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
void SDLWrapper::logSDLVersion ( const std::string& what_, const SDL_version& compiled_, std::string revision_ )
{	
	Log(DEBUG) << what_ << " Version (Compiled): " << ( int )compiled_.major << "." << ( int )compiled_.minor << "." 
	  << ( int )compiled_.patch << ( ( !revision_.empty() ) ? revision_ : "" );
}
