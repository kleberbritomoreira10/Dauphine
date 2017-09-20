/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @GStateOptions.cpp
* The control of state of the options of the game.
* Class that controll the state of options of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "GStateOptions.h"
#include "LuaScript.h"
#include "Game.h"

#include "Util.h"

#include <string>

const std::string GStateOptions::possibleResolutions[3] = {"800x600", "768x432", "960x540"};

/**
* The constructor.
* Initializes the attributes.
*/
GStateOptions::GStateOptions() :
	elapsedTime( 0.0 ),
	optionsImage( nullptr ),
	currentResolution( R_960_540 ),
	currentOption( O_RESOLUTION ),
	selector( nullptr ),
	selectorXPositionLeft{ 780, 780, 780, 590, 590 },
	selectorYPositionLeft{ 365, 468, 580, 665, 750 },
	selectorXPositionRight{ 1010, 1010, 1010, 900, 900},
	selectorYPositionRight{ 365, 468, 580, 665, 750 },
	musicVolume( 100 ),
	sfxVolume( 100 ),
	resolution( nullptr ),
	volumeMusic( nullptr ),
	volumeSFX( nullptr )	
{
	this -> resolution = new Text( 830.0, 365.0, "res/fonts/maturasc.ttf", 45, \
		possibleResolutions[currentResolution].c_str() );
	this -> volumeMusic = new Text( 830.0, 468.0, "res/fonts/maturasc.ttf", 45, \
		Util::toString( this -> musicVolume ).c_str() );
	this -> volumeSFX = new Text( 830.0, 580.0, "res/fonts/maturasc.ttf", 45, \
		Util::toString( this -> sfxVolume ).c_str() );
}

/**
* The destructor.
*/
GStateOptions::~GStateOptions()
{
	// Deleting resolution.
	if( this -> resolution != nullptr )
	{
		delete this -> resolution;
		this -> resolution = nullptr;
	}

	// Deleting volume of music.	
	if( this -> volumeMusic != nullptr )
	{
		delete this -> volumeMusic;
		this -> volumeMusic = nullptr;
	}

	// Deleting volume of selector.	
	if( this -> volumeSFX != nullptr )
	{
		delete this -> volumeSFX;
		this -> volumeSFX = nullptr;
	}
}

/**
* Changing the state for options of the game.
*/
void GStateOptions::update( const double dt_ )
{
	this -> elapsedTime += dt_;

	this -> resolution -> changeText( possibleResolutions[ currentResolution ].c_str() );
	this -> volumeMusic -> changeText( Util::toString( this -> musicVolume).c_str() );
	this -> volumeSFX -> changeText( Util::toString( this -> sfxVolume ).c_str() );

	const std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	// Verifying if the keys for escape are valids.
	if( keyStates[ GameKeys::ESCAPE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );
	}

	const double selectorDelayTime = 0.2;

	// Verifying if the keys for state down are valids.
	if( keyStates[ GameKeys::DOWN ] == true )
	{
		// Verifying if time used for select a option is higher than a expected by default.
		if( this -> elapsedTime >= selectorDelayTime )
		{
			// Verifying if the current option is equal of the last of the list.
			if( this -> currentOption == (O_TOTAL - 1 ) )
			{
				this -> currentOption = O_RESOLUTION;
			}

			// Verifying if the current option is different of the last of the list.			
			else
			{
				this -> currentOption++;
			}
			this -> elapsedTime = 0.0;
		}
	}

	// Verifying if the keys for state up are valids.	
	if( keyStates[ GameKeys::UP ] == true )
	{
		// Verifying if time used for select a option is higher than a expected by default.		
		if( this -> elapsedTime >= selectorDelayTime)
		{
			// Verifying if the current option is equal of the first of the list.			
			if( this -> currentOption == O_RESOLUTION )
			{
				this -> currentOption = ( O_TOTAL - 1 );
			}
			// Verifying if the current option is different of the first of the list.
			else
			{
				this -> currentOption--;
			}
			this -> elapsedTime = 0.0;
		}
	}

	// Verifying if the keys for state left are valids.
	if( keyStates[ GameKeys::LEFT ] == true)
	{
		// Verifying if time used for select a option is higher than a expected by default.				
		if( this->elapsedTime >= selectorDelayTime )
		{
			// Verifying if the current option is equal of the resolution.			
			if( this -> currentOption == O_RESOLUTION )
			{
				// Verifying if the current resolution is equal the expected.
				if( this -> currentResolution == R_800_600 )
				{
					this -> currentResolution = ( R_TOTAL - 1 );
				}
				// Verifying if the current resolution is different of the expected.				
				else
				{
					this -> currentResolution--;
				}
			}

			// Verifying if the current option is equal of the volume of the music.			
			else if( this -> currentOption == O_VOLUME_MUSIC )
			{
				// Verifying if the volume of music is higher than zero.				
				if( this -> musicVolume > 0 )
				{	
					this -> musicVolume -= 5;
				}
			}

			// Verifying if the current option is equal of the volume of the selector.						
			else if( this -> currentOption == O_VOLUME_SFX )
			{
				// Verifying if the volume of selector is higher than zero.				
				if( this -> sfxVolume > 0)
				{
					this -> sfxVolume -= 5;
				}
			}

			this -> elapsedTime = 0.0;
		}
	}

	// Verifying if the keys for state right are valids.	
	if( keyStates[ GameKeys::RIGHT ] == true )
	{
		// Verifying if time used for select a option is higher than a expected by default.		
		if( this -> elapsedTime >= selectorDelayTime )
		{
			// Verifying if the current option is equal of the resolution.			
			if( this -> currentOption == O_RESOLUTION )
			{
				// Verifying if the current resolution is equal of the larger value possible.				
				if( this -> currentResolution == ( R_TOTAL - 1 ) )
				{
					this -> currentResolution = R_800_600;
				}
				// Verifying if the current resolution is different of the larger value possible.								
				else
				{
					this -> currentResolution++;
				}
			}

			// Verifying if the current option is equal of the volume of the music.						
			else if( this -> currentOption == O_VOLUME_MUSIC )
			{
				// Verifying if the volume of music is smaller than a hundread.				
				if( this -> musicVolume < 100 )
				{
					this -> musicVolume += 5;
				}
			}

			// Verifying if the current option is equal of the volume of the selector.
			else if( this -> currentOption == O_VOLUME_SFX )
			{
				if( this -> sfxVolume < 100 )
				{
					this -> sfxVolume += 5;
				}
			}

			// Verifying if the option is different of the cases implemented.
			else
			{
				// Condition not implemented
			}

			this -> elapsedTime = 0.0;
		}
	}

	// Verifying if the keys for state space are valids and have not been applied.	
	if( keyStates[ GameKeys::SPACE ] == true && this -> currentOption == O_APPLY )
	{
		applyOptions();
	}

	// Verifying if the keys for state space are valids and have been applied.		
	if( keyStates[ GameKeys::SPACE ] == true && this -> currentOption == O_RETURN )
	{
		Game::instance().setState( Game::GStates::MENU );
	}
}

/**
* Showing the options of the game.
*/
void GStateOptions::render()
{
	// Verifying if the option of image is not null.
	if( this -> optionsImage != nullptr )
	{
		this -> optionsImage -> render( 0, 0, nullptr, true );
	}

	// Verifying if the option of image is null.	
	else
	{
		Log( WARN ) << "No image set for the options screen!";
	}

	this -> resolution -> render( 0, 0 );
	this -> volumeMusic -> render( 0, 0 );
	this -> volumeSFX -> render( 0, 0 );

	// Verifying if the selector is not null.	
	if( this -> selector != nullptr )
	{
		this -> selector -> render( selectorXPositionLeft[ currentOption ],
			selectorYPositionLeft[ currentOption ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

		this -> selector -> render( selectorXPositionRight[ currentOption ],
			selectorYPositionRight[ currentOption ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );
	}

	// Verifying if the option of image is null.	
	else
	{
		Log( WARN ) << "No image set for the selector.";
	}

}

/**
* Load the options of the game.
*/
void GStateOptions::load()
{
	Log( DEBUG ) << "Loading options...";

	LuaScript luaOptions("lua/Options.lua");
	const std::string pathOptions = luaOptions.unlua_get<std::string>("options.images.dummy");
	const std::string pathCursor = luaOptions.unlua_get<std::string>("options.images.cursor");

	this -> currentResolution = R_960_540;
	this -> currentOption = O_RESOLUTION;
	
    this -> optionsImage = Game::instance().getResources().get( pathOptions );
    this -> selector = Game::instance().getResources().get( pathCursor );

    this -> selector -> setWidth( 50 );
}

/**
* Unloading the game options and cleaning the display.
*/
void GStateOptions::unload()
{
	Log( DEBUG ) << "\tUnloading options...";
	cleanEntities();
}

/**
* Applying the options in the game.
*/
void GStateOptions::applyOptions()
{
	// Verifying if the resolution is equal a 800x600.	
	if( this -> currentResolution == R_800_600 )
	{
		Game::instance().resizeWindow( 800, 600 );
	}

	// Verifying if the resolution is equal a 768x432.		
	else if( this -> currentResolution == R_768_432 )
	{
		Game::instance().resizeWindow( 768, 432 );
	}

	// Verifying if the resolution is equal a 960x540.		
	else if( this -> currentResolution == R_960_540 )
	{
		Game::instance().resizeWindow( 960, 540 );
	}

	// Apply volume music
	Game::instance().getAudioHandler().setMusicVolume( this -> musicVolume );

	// Apply volume sfx
	Game::instance().getAudioHandler().setEffectVolume( this -> sfxVolume );
}
