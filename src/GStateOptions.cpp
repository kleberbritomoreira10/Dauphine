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

#define TEXT_SIZE 45
#define INITIAL_TIME 0
#define INITIAL_VOLUME 0
#define MAX_VOLUME 100
#define WIDTH_VALUE 50
#define POSITION_X 0
#define POSITION_Y 0

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
	this -> resolution = new Text( 830.0, 365.0, "res/fonts/maturasc.ttf", TEXT_SIZE, \
		possible_resolutions[current_resolution].c_str() ); // Load option text resolution.

	this -> music_volume_text = new Text( 830.0, 468.0, "res/fonts/maturasc.ttf", TEXT_SIZE, \
		Util::toString( this -> music_volume ).c_str() ); // Load the music volume of the options menu.

	this -> sfx_volume_text = new Text( 830.0, 580.0, "res/fonts/maturasc.ttf", TEXT_SIZE, \
		Util::toString( this -> sfx_volume ).c_str() ); // Load the sfx volume of the options menu.
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

// Delete all the resolutions.
void GStateOptions::delete_resolution()
{
	if( this -> resolution != nullptr )
	{
		delete this -> resolution; // Delete the resolution instance.
		this -> resolution = nullptr;

	}else
	{
		Log( DEBUG ) << "Resolution is already null";
	}
}

// Delete all the music volume.
void GStateOptions::delete_music_volume_text()
{
	if( this -> music_volume_text != nullptr )
	{
		delete this -> music_volume_text; // Delete the music volume instance.
		this -> music_volume_text = nullptr;

	}else
	{
		Log( DEBUG ) << "Music volume is already null.";
	}
}

// Delete sfx volume.
void GStateOptions::delete_sfx_volume_text()
{
	if( this -> sfx_volume_text != nullptr )
	{
		delete this -> sfx_volume_text; // Delete sfx volume.
		this -> sfx_volume_text = nullptr;

	}else
	{
		Log( DEBUG ) << "Sfx volume is already null.";
	}
}

/**
* Changing the state for options of the game.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
* 	of processing speed.
*/
void GStateOptions::update( const double DELTA_TIME )
{
	assert( DELTA_TIME > INITIAL_TIME ); // Check there is a time variation.
	this -> elapsedTime += DELTA_TIME;

	// Update the options menu resolution.
	this -> resolution -> changeText( possible_resolutions[ current_resolution ].c_str() );

	//Change the options menu music volume.
	this -> music_volume_text -> changeText( Util::toString( this -> music_volume).c_str() );

	// Change the options menu sfx volume.
	this -> sfx_volume_text -> changeText( Util::toString( this -> sfx_volume ).c_str() );

	const std::array< bool, GameKeys::MAX > keyStates = Game::instance().getInput(); // Get key states from input.

	if( keyStates[ GameKeys::ESCAPE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );

	}else
	{
		Log( DEBUG ) << "State ESCAPE is false";
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
		Log( DEBUG ) << "Invalid Key State";
	}
}

// Handle the options when the key is DOWN.
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
		Log( DEBUG ) << "elapsedTime <= SELECTOR_DELAY_TIME";
	}
}

// Handle the options when the key is UP.
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
		Log( DEBUG ) << "elapsedTime <= SELECTOR_DELAY_TIME";
	}
}

// Handle the options when the key is LEFT.
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
			if( this -> music_volume > INITIAL_VOLUME )
			{
				this -> music_volume -= 5;

			}else
			{
				Log( DEBUG ) << "Music volume is 0.";
			}

		}else if( this -> current_option == OPTIONS_VOLUME_SFX ) // Option == VOLUME SFX
		{
			if( this -> sfx_volume > INITIAL_VOLUME )
			{
				this -> sfx_volume -= 5;

			}else
			{
				Log( DEBUG ) << "Sfx volume is 0.";
			}
		}else
		{
			Log( DEBUG ) << "Invalid option.";
		}

		this -> elapsedTime = 0.0;
	}
}

// Handle the options when the key is RIGHT.
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
			if( this -> music_volume < MAX_VOLUME )
			{
				this -> music_volume += 5;

			}else
			{
				Log( DEBUG ) << "Max volume exeeded";
			}

		}else if( this -> current_option == OPTIONS_VOLUME_SFX ) // Option == VOLUME SFX
		{
			if( this -> sfx_volume < MAX_VOLUME )
			{
				this -> sfx_volume += 5;

			}else
			{
				Log( DEBUG ) << "Max sfx volume exeeded.";
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

	this -> resolution -> render( POSITION_X, POSITION_Y ); // Render the options menu resolution.
	this -> music_volume_text -> render( POSITION_X, POSITION_Y ); // Render the options menu music volume text.
	this -> sfx_volume_text -> render( POSITION_X, POSITION_Y ); // Render the options menu sfx volume text.

	render_selector();

}

void GStateOptions::render_options_image()
{
	if( this -> options_image != nullptr )
	{
		this -> options_image -> render( POSITION_X, POSITION_Y, nullptr, true ); // Render the options image.
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
			// Renders the selector of the options menu with none flip.
			this -> selector -> render( selector_X_position_left[ current_option ],
				selector_Y_position_left[ current_option ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );
			
			// Renders the selector with horizontal flip.
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

	LuaScript luaOptions( "lua/Options.lua" ); // Load options from LUA.

	// Path to options image.
	const std::string path_options = luaOptions.unlua_get< std::string >( "options.images.dummy" );

	// Path to options cursor image.
	const std::string path_cursor = luaOptions.unlua_get< std::string >( "options.images.cursor" );

	this -> current_resolution = RESOLUTION_960_540; // Setting current resolution to 960x540.
	this -> current_option = OPTIONS_RESOLUTION; // Setting options resolution.

  this -> options_image = Game::instance().getResources().get( path_options ); // Getting options image resource.
  this -> selector = Game::instance().getResources().get( path_cursor ); // Getting selector image resource.

  this -> selector -> setWidth( WIDTH_VALUE ); // Setting selector width
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
		Game::instance().resizeWindow( resolution_X, resolution_Y ); // Resizes the window with the 800x600 resolution.

	}else if( this -> current_resolution == RESOLUTION_768_432 )
	{

		int resolution_X = 768;
		int resolution_Y = 432;
		Game::instance().resizeWindow( resolution_X, resolution_Y ); // Resizes the window with the 768x432 resolution.

	}else if( this -> current_resolution == RESOLUTION_960_540 )
	{

		int resolution_X = 960;
		int resolution_Y = 540;
		Game::instance().resizeWindow( resolution_X, resolution_Y ); // Resizes the window with the 960x540 resolution.

	}else
	{
		Log( DEBUG ) << "Invalid Resolution!!";
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
