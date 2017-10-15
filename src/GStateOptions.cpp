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
	selector_X_position_left{ 780, 780, 780, 590, 590 },
	selector_Y_position_left{ 365, 468, 580, 665, 750 },
	selector_X_position_right{ 1010, 1010, 1010, 900, 900},
	selector_Y_position_right{ 365, 468, 580, 665, 750 },
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
	if( this -> resolution != nullptr )
	{
		delete this -> resolution;
		this -> resolution = nullptr;
	}

	if( this -> volumeMusic != nullptr )
	{
		delete this -> volumeMusic;
		this -> volumeMusic = nullptr;
	}

	if( this -> volumeSFX != nullptr )
	{
		delete this -> volumeSFX;
		this -> volumeSFX = nullptr;
	}
}

/**
* Changing the state for options of the game.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
* 	of processing speed.
*/
void GStateOptions::update( const double DELTA_TIME )
{
	this -> elapsedTime += DELTA_TIME;

	this -> resolution -> changeText( possibleResolutions[ currentResolution ].c_str() );
	this -> volumeMusic -> changeText( Util::toString( this -> musicVolume).c_str() );
	this -> volumeSFX -> changeText( Util::toString( this -> sfxVolume ).c_str() );

	const std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	if( keyStates[ GameKeys::ESCAPE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );
	}

	const double SELECTOR_DELAY_TIME = 0.2;

	if( keyStates[ GameKeys::DOWN ] == true )
	{
		if( this -> elapsedTime >= SELECTOR_DELAY_TIME )
		{
			if( this -> currentOption == (O_TOTAL - 1 ) )
			{
				this -> currentOption = O_RESOLUTION;
			}
			else
			{
				this -> currentOption++;
			}
			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::UP ] == true )
	{
		if( this -> elapsedTime >= SELECTOR_DELAY_TIME)
		{
			if( this -> currentOption == O_RESOLUTION )
			{
				this -> currentOption = ( O_TOTAL - 1 );
			}
			else
			{
				this -> currentOption--;
			}
			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::LEFT ] == true)
	{
		if( this->elapsedTime >= SELECTOR_DELAY_TIME )
		{
			// Option == Resolution
			if( this -> currentOption == O_RESOLUTION )
			{
				if( this -> currentResolution == R_800_600 )
				{
					this -> currentResolution = ( R_TOTAL - 1 );
				}
				else
				{
					this -> currentResolution--;
				}
			}
			// Option == VOLUME MUSIC
			else if( this -> currentOption == O_VOLUME_MUSIC )
			{
				if( this -> musicVolume > 0 )
				{	
					this -> musicVolume -= 5;
				}
			}
			// Option == VOLUME SFX
			else if( this -> currentOption == O_VOLUME_SFX )
			{
				if( this -> sfxVolume > 0)
				{
					this -> sfxVolume -= 5;
				}
			}

			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::RIGHT ] == true )
	{
		if( this -> elapsedTime >= SELECTOR_DELAY_TIME )
		{
			// Option == Resolution
			if( this -> currentOption == O_RESOLUTION )
			{
				if( this -> currentResolution == ( R_TOTAL - 1 ) )
				{
					this -> currentResolution = R_800_600;
				}
				else
				{
					this -> currentResolution++;
				}
			}
			// Option == VOLUME MUSIC
			else if( this -> currentOption == O_VOLUME_MUSIC )
			{
				if( this -> musicVolume < 100 )
				{
					this -> musicVolume += 5;
				}
			}
			// Option == VOLUME SFX
			else if( this -> currentOption == O_VOLUME_SFX )
			{
				if( this -> sfxVolume < 100 )
				{
					this -> sfxVolume += 5;
				}
			}
			else
			{
				// Condition not implemented
			}

			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::SPACE ] == true && this -> currentOption == O_APPLY )
	{
		applyOptions();
	}

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
	if( this -> optionsImage != nullptr )
	{
		this -> optionsImage -> render( 0, 0, nullptr, true );
	}
	else
	{
		Log( WARN ) << "No image set for the options screen!";
	}

	this -> resolution -> render( 0, 0 );
	this -> volumeMusic -> render( 0, 0 );
	this -> volumeSFX -> render( 0, 0 );

	if( this -> selector != nullptr )
	{
		this -> selector -> render( selector_X_position_left[ currentOption ],
			selector_Y_position_left[ currentOption ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

		this -> selector -> render( selector_X_position_right[ currentOption ],
			selector_Y_position_right[ currentOption ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );
	}
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
	clean_entities();
}

/**
* Applying the options in the game.
*/
void GStateOptions::applyOptions()
{
	// Apply resolution
	if( this -> currentResolution == R_800_600 )
	{
		Game::instance().resizeWindow( 800, 600 );
	}
	else if( this -> currentResolution == R_768_432 )
	{
		Game::instance().resizeWindow( 768, 432 );
	}
	else if( this -> currentResolution == R_960_540 )
	{
		Game::instance().resizeWindow( 960, 540 );
	}

	// Apply volume music
	Game::instance().get_audio_handler().setMusicVolume( this -> musicVolume );

	// Apply volume sfx
	Game::instance().get_audio_handler().setEffectVolume( this -> sfxVolume );
}
