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

const std::string GStateOptions::possible_resolutions[3] = {"800x600", "768x432", "960x540"};

/**
* The constructor.
* Initializes the attributes.
*/
GStateOptions::GStateOptions() :
	elapsedTime( 0.0 ),
	options_image( nullptr ),
	current_resolution( R_960_540 ),
	current_option( O_RESOLUTION ),
	selector( nullptr ),
	selector_X_position_left{ 780, 780, 780, 590, 590 },
	selector_Y_position_left{ 365, 468, 580, 665, 750 },
	selector_X_position_right{ 1010, 1010, 1010, 900, 900},
	selector_Y_position_right{ 365, 468, 580, 665, 750 },
	music_volume( 100 ),
	sfx_volume( 100 ),
	resolution( nullptr ),
	music_volume_text( nullptr ),
	sfx_volume_text( nullptr )	
{
	this -> resolution = new Text( 830.0, 365.0, "res/fonts/maturasc.ttf", 45, \
		possible_resolutions[current_resolution].c_str() );
	this -> music_volume_text = new Text( 830.0, 468.0, "res/fonts/maturasc.ttf", 45, \
		Util::toString( this -> music_volume ).c_str() );
	this -> sfx_volume_text = new Text( 830.0, 580.0, "res/fonts/maturasc.ttf", 45, \
		Util::toString( this -> sfx_volume ).c_str() );
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

	if( this -> music_volume_text != nullptr )
	{
		delete this -> music_volume_text;
		this -> music_volume_text = nullptr;
	}

	if( this -> sfx_volume_text != nullptr )
	{
		delete this -> sfx_volume_text;
		this -> sfx_volume_text = nullptr;
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

	this -> resolution -> changeText( possible_resolutions[ current_resolution ].c_str() );
	this -> music_volume_text -> changeText( Util::toString( this -> music_volume).c_str() );
	this -> sfx_volume_text -> changeText( Util::toString( this -> sfx_volume ).c_str() );

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
			if( this -> current_option == (O_TOTAL - 1 ) )
			{
				this -> current_option = O_RESOLUTION;
			}
			else
			{
				this -> current_option++;
			}
			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::UP ] == true )
	{
		if( this -> elapsedTime >= SELECTOR_DELAY_TIME)
		{
			if( this -> current_option == O_RESOLUTION )
			{
				this -> current_option = ( O_TOTAL - 1 );
			}
			else
			{
				this -> current_option--;
			}
			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::LEFT ] == true)
	{
		if( this->elapsedTime >= SELECTOR_DELAY_TIME )
		{
			// Option == Resolution
			if( this -> current_option == O_RESOLUTION )
			{
				if( this -> current_resolution == R_800_600 )
				{
					this -> current_resolution = ( R_TOTAL - 1 );
				}
				else
				{
					this -> current_resolution--;
				}
			}
			// Option == VOLUME MUSIC
			else if( this -> current_option == O_VOLUME_MUSIC )
			{
				if( this -> music_volume > 0 )
				{	
					this -> music_volume -= 5;
				}
			}
			// Option == VOLUME SFX
			else if( this -> current_option == O_VOLUME_SFX )
			{
				if( this -> sfx_volume > 0)
				{
					this -> sfx_volume -= 5;
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
			if( this -> current_option == O_RESOLUTION )
			{
				if( this -> current_resolution == ( R_TOTAL - 1 ) )
				{
					this -> current_resolution = R_800_600;
				}
				else
				{
					this -> current_resolution++;
				}
			}
			// Option == VOLUME MUSIC
			else if( this -> current_option == O_VOLUME_MUSIC )
			{
				if( this -> music_volume < 100 )
				{
					this -> music_volume += 5;
				}
			}
			// Option == VOLUME SFX
			else if( this -> current_option == O_VOLUME_SFX )
			{
				if( this -> sfx_volume < 100 )
				{
					this -> sfx_volume += 5;
				}
			}
			else
			{
				// Condition not implemented
			}

			this -> elapsedTime = 0.0;
		}
	}

	if( keyStates[ GameKeys::SPACE ] == true && this -> current_option == O_APPLY )
	{
		applyOptions();
	}

	if( keyStates[ GameKeys::SPACE ] == true && this -> current_option == O_RETURN )
	{
		Game::instance().setState( Game::GStates::MENU );
	}
}

/**
* Showing the options of the game.
*/
void GStateOptions::render()
{
	if( this -> options_image != nullptr )
	{
		this -> options_image -> render( 0, 0, nullptr, true );
	}
	else
	{
		Log( WARN ) << "No image set for the options screen!";
	}

	this -> resolution -> render( 0, 0 );
	this -> music_volume_text -> render( 0, 0 );
	this -> sfx_volume_text -> render( 0, 0 );

	if( this -> selector != nullptr )
	{
		this -> selector -> render( selector_X_position_left[ current_option ],
			selector_Y_position_left[ current_option ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

		this -> selector -> render( selector_X_position_right[ current_option ],
			selector_Y_position_right[ current_option ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );
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
	const std::string path_cursor = luaOptions.unlua_get<std::string>("options.images.cursor");

	this -> current_resolution = R_960_540;
	this -> current_option = O_RESOLUTION;
	
    this -> options_image = Game::instance().getResources().get( pathOptions );
    this -> selector = Game::instance().getResources().get( path_cursor );

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
	if( this -> current_resolution == R_800_600 )
	{
		Game::instance().resizeWindow( 800, 600 );
	}
	else if( this -> current_resolution == R_768_432 )
	{
		Game::instance().resizeWindow( 768, 432 );
	}
	else if( this -> current_resolution == R_960_540 )
	{
		Game::instance().resizeWindow( 960, 540 );
	}

	// Apply volume music
	Game::instance().get_audio_handler().setMusicVolume( this -> music_volume );

	// Apply volume sfx
	Game::instance().get_audio_handler().setEffectVolume( this -> sfx_volume );
}
