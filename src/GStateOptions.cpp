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
#include <assert.h>

const std::string GStateOptions::possible_resolutions[ 3 ] = { "800x600", "768x432", "960x540" };

/**
* The constructor.
* Initializes the attributes.
*/
GStateOptions::GStateOptions() :
	elapsedTime( 0.0 ),
	options_image( nullptr ),
	current_resolution( RESOLUTION_960_540 ),
	current_option( OPTIONS_RESOLUTION ),
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
	delete_resolution();

	delete_music_volume_text();

	delete_sfx_volume_text();
}

void GStateOptions::delete_resolution()
{
	if( this -> resolution != nullptr )
	{
		delete this -> resolution;
		this -> resolution = nullptr;

	}else
	{
		// No action.
	}
}

void GStateOptions::delete_music_volume_text()
{
	if( this -> music_volume_text != nullptr )
	{
		delete this -> music_volume_text;
		this -> music_volume_text = nullptr;

	}else
	{
		// No action.
	}
}

void GStateOptions::delete_sfx_volume_text()
{
	if( this -> sfx_volume_text != nullptr )
	{
		delete this -> sfx_volume_text;
		this -> sfx_volume_text = nullptr;

	}else
	{
		// No action.
	}
}

/**
* Changing the state for options of the game.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
* 	of processing speed.
*/
void GStateOptions::update( const double DELTA_TIME )
{
	assert( DELTA_TIME > 0 );
	this -> elapsedTime += DELTA_TIME;

	this -> resolution -> changeText( possible_resolutions[ current_resolution ].c_str() );
	this -> music_volume_text -> changeText( Util::toString( this -> music_volume).c_str() );
	this -> sfx_volume_text -> changeText( Util::toString( this -> sfx_volume ).c_str() );

	const std::array< bool, GameKeys::MAX > keyStates = Game::instance().getInput();

	if( keyStates[ GameKeys::ESCAPE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );

	}else
	{
		// No action.
	}

	const double SELECTOR_DELAY_TIME = 0.2;

	if( keyStates[ GameKeys::DOWN ] == true )
	{

		handle_current_option_down_key( SELECTOR_DELAY_TIME );

	}else if( keyStates[ GameKeys::UP ] == true )
	{

		handle_current_option_up_key( SELECTOR_DELAY_TIME );

	}else if( keyStates[ GameKeys::LEFT ] == true)
	{

		handle_current_option_left_key( SELECTOR_DELAY_TIME );

	}else if( keyStates[ GameKeys::RIGHT ] == true )
	{

		handle_current_option_right_key( SELECTOR_DELAY_TIME );

	}else if( keyStates[ GameKeys::SPACE ] == true && this -> current_option == OPTIONS_APPLY )
	{
		applyOptions();

	}else if( keyStates[ GameKeys::SPACE ] == true && this -> current_option == OPTIONS_RETURN )
	{
		Game::instance().setState( Game::GStates::MENU );

	}else
	{
		// No action.
	}
}


void GStateOptions::handle_current_option_down_key( const double SELECTOR_DELAY_TIME )
{
	if( this -> elapsedTime >= SELECTOR_DELAY_TIME )
	{
		if( this -> current_option == ( OPTIONS_TOTAL - 1 ) )
		{
			this -> current_option = OPTIONS_RESOLUTION;
		}
		else
		{
			this -> current_option++;
		}

		this -> elapsedTime = 0.0;

	}else
	{
		// No action.
	}
}

void GStateOptions::handle_current_option_up_key( const double SELECTOR_DELAY_TIME )
{
	if( this -> elapsedTime >= SELECTOR_DELAY_TIME)
	{
		if( this -> current_option == OPTIONS_RESOLUTION )
		{
			this -> current_option = ( OPTIONS_TOTAL - 1 );

		}else
		{
			this -> current_option--;
		}

		this -> elapsedTime = 0.0;

	}else
	{
		// No action.
	}
}

void GStateOptions::handle_current_option_left_key( const double SELECTOR_DELAY_TIME )
{
	if( this->elapsedTime >= SELECTOR_DELAY_TIME )
	{
		// Option == Resolution
		if( this -> current_option == OPTIONS_RESOLUTION )
		{
			if( this -> current_resolution == RESOLUTION_800_600 )
			{
				this -> current_resolution = ( RESOLUTION_TOTAL - 1 );
			}
			else
			{
				this -> current_resolution--;
			}

		}else if( this -> current_option == OPTIONS_VOLUME_MUSIC ) // Option == VOLUME MUSIC
		{
			if( this -> music_volume > 0 )
			{
				this -> music_volume -= 5;

			}else
			{
				//No action.
			}

		}else if( this -> current_option == OPTIONS_VOLUME_SFX ) // Option == VOLUME SFX
		{
			if( this -> sfx_volume > 0)
			{
				this -> sfx_volume -= 5;

			}else
			{
				// No action.
			}
		}else
		{
			// No action.
		}

		this -> elapsedTime = 0.0;
	}
}

void GStateOptions::handle_current_option_right_key( const double SELECTOR_DELAY_TIME )
{
	if( this -> elapsedTime >= SELECTOR_DELAY_TIME )
	{
		// Option == Resolution
		if( this -> current_option == OPTIONS_RESOLUTION )
		{
			if( this -> current_resolution == ( RESOLUTION_TOTAL - 1 ) )
			{
				this -> current_resolution = RESOLUTION_800_600;
			}
			else
			{
				this -> current_resolution++;
			}

		}else if( this -> current_option == OPTIONS_VOLUME_MUSIC ) // Option == VOLUME MUSIC
		{
			if( this -> music_volume < 100 )
			{
				this -> music_volume += 5;

			}else
			{
				// No action.
			}

		}else if( this -> current_option == OPTIONS_VOLUME_SFX ) // Option == VOLUME SFX
		{
			if( this -> sfx_volume < 100 )
			{
				this -> sfx_volume += 5;

			}else
			{
				// No action.
			}

		}else
		{
			// Condition not implemented
		}

		this -> elapsedTime = 0.0;
	}
}

/**
* Showing the options of the game.
*/
void GStateOptions::render()
{
	render_options_image();

	this -> resolution -> render( 0, 0 );
	this -> music_volume_text -> render( 0, 0 );
	this -> sfx_volume_text -> render( 0, 0 );

	render_selector();

}

void GStateOptions::render_options_image()
{
	if( this -> options_image != nullptr )
	{
		this -> options_image -> render( 0, 0, nullptr, true );
	}
	else
	{
		Log( WARN ) << "No image set for the options screen!";
	}
}

void GStateOptions::render_selector()
{
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

	LuaScript luaOptions( "lua/Options.lua" );
	const std::string path_options = luaOptions.unlua_get< std::string >( "options.images.dummy" );
	const std::string path_cursor = luaOptions.unlua_get< std::string >( "options.images.cursor" );

	this -> current_resolution = RESOLUTION_960_540;
	this -> current_option = OPTIONS_RESOLUTION;

  this -> options_image = Game::instance().getResources().get( path_options );
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
	if( this -> current_resolution == RESOLUTION_800_600 )
	{

		int resolution_X = 800;
		int resolution_Y = 600;
		Game::instance().resizeWindow( resolution_X, resolution_Y );

	}else if( this -> current_resolution == RESOLUTION_768_432 )
	{

		int resolution_X = 768;
		int resolution_Y = 432;
		Game::instance().resizeWindow( resolution_X, resolution_Y );

	}else if( this -> current_resolution == RESOLUTION_960_540 )
	{

		int resolution_X = 960;
		int resolution_Y = 540;
		Game::instance().resizeWindow( resolution_X, resolution_Y );

	}else
	{
		// No action.
	}

	apply_volume();
}

void GStateOptions::apply_volume()
{
	// Apply volume music
	Game::instance().get_audio_handler().setMusicVolume( this -> music_volume );

	// Apply volume sfx
	Game::instance().get_audio_handler().setEffectVolume( this -> sfx_volume );
}
