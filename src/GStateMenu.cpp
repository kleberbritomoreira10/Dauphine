/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @GStateMenu.cpp
* The control of state of the menu of the game.
* Class that controll the state of menu of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "GStateMenu.h"
#include "LuaScript.h"
#include "Game.h"

#include <string>

/**
* The constructor.
* Initializes the attributes.
*/
GStateMenu::GStateMenu() :
	shouldIgnore( false ),
	menu_image( nullptr ),
	menu_selector( nullptr ),
	attrack_mode_background( nullptr ),
	attract_mode( nullptr ),
	passed_time( 0.0 ),
	current_selection( Selection::NEWGAME ),
	selector_X_position_left { 610, 635, 635, 645 },
	selector_Y_position_left { 560, 625, 690, 755 },
	selector_X_position_right { 880, 855, 855, 845 },
	selector_Y_position_right { 560, 625, 690, 755 },
	attract_height_size( 600 ),
	attract_change_speed( 1 ),
	attractClip{ 0, 0, 0, this -> attract_height_size },
	shwing_animation( nullptr ),
	is_shwing_activated( true ),
	shwing( nullptr ),
	shwing_clip { 0,0,0,0 }
{

}

/**
* The destructor.
*/
GStateMenu::~GStateMenu()
{
	if( this -> shwing_animation != nullptr )
	{
		delete this -> shwing_animation;
		this -> shwing_animation = nullptr;
	}
}

/**
* Load the menu of the game showing the options and playing the music.
*/
void GStateMenu::load()
{
	Log( DEBUG ) << "Loading menu...";

	// Changing the music.
	Game::instance().clearKeyFromInput(GameKeys::ESCAPE);

	Game::instance().get_audio_handler().change_music("res/audio/menu.mid");

	LuaScript luaMenu("lua/Menu.lua");
	const std::string pathTitleScreen = luaMenu.unlua_get<std::string>("menu.images.titleScreen");
	const std::string pathCursor = luaMenu.unlua_get<std::string>("menu.images.cursor");

    this -> menu_image = Game::instance().get_resources().get( pathTitleScreen );
    this -> menu_selector = Game::instance().get_resources().get( pathCursor );
    this -> attrack_mode_background = Game::instance().get_resources().get("res/images/title_background.png");
    this -> attract_mode = Game::instance().get_resources().get("res/images/attract.png");
    this -> attractClip.w = this -> attract_mode -> getWidth();
    this -> shwing_animation = new Animation( 0, 0, 795, 360, 3, false );
    this -> shwing = Game::instance().get_resources().get("res/images/shwing_sheet.png");
    this -> shwing_animation -> ANIMATION_LIMIT = 2;

    Game::instance().get_fade().fade_out( 0, 0.002 );
}

/**
* Exiting of the menu and cleaning the display.
*/
void GStateMenu::unload()
{
	Log( DEBUG ) << "\tUnloading menu...";
	this -> attractClip.y = 0;
	clean_entities();
}

/**
* Changing the state menu of the game.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
* 	of processing speed.
*/
void GStateMenu::update( const double DELTA_TIME )
{
	this -> passed_time += DELTA_TIME;

	handleSelectorMenu();

	this -> shwing_animation -> update( this -> shwing_clip, DELTA_TIME );

	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();
	if( keyStates[ GameKeys::ESCAPE ] == true )
	{
		Game::instance().stop();
	}

	if( this -> is_shwing_activated )
	{
		 this -> shwing_animation -> change_animation( 0, 0, 12, false, 2 );
		 this -> is_shwing_activated = false;
	}

}

/**
* Show the menu of the game with your properties.
*/
void GStateMenu::render()
{

	if( this -> passed_time > 10 )
	{
		this -> attrack_mode_background -> render( 0, 0, nullptr, true );
		this -> attract_mode -> render( 0, 0, &this -> attractClip, true );
		shouldIgnore = true;
		if( this -> attractClip.y < ( int ) this -> attract_mode -> getHeight() - this -> attract_height_size )
		{
			this -> attractClip.y  += this -> attract_change_speed;
		}
		else
		{
			//shwing->render(340,50,&this->shwing_clip);
		}
		if( this -> passed_time > 75 )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
		}
	}
	else
	{
		if( this -> menu_image != nullptr )
		{
			this -> menu_image -> render( 0, 0, nullptr, true );

			this -> menu_selector -> setWidth( 50 );

			this -> menu_selector -> render( selector_X_position_left[ current_selection ],
				selector_Y_position_left[ current_selection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

			this -> menu_selector -> render( selector_X_position_right[ current_selection ],
				selector_Y_position_right[ current_selection ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );

		}
		else
		{
			Log( WARN ) << "No image set to display on the menu!";
		}
	}

}

/**
* Controlls the user input for enter in menu of the game.
*/
void GStateMenu::handleSelectorMenu()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	const double SELECTOR_DELAY_TIME = 0.2;

	if( keyStates[ GameKeys::DOWN ] == true || keyStates[ GameKeys::RIGHT ] == true )
	{

		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		if( this -> passed_time >= SELECTOR_DELAY_TIME )
		{
			if( current_selection < ( Selection::TOTAL - 1 ) )
			{
				current_selection++;
			}
			else
			{
				current_selection = Selection::NEWGAME;
			}

			this->passed_time = 0.0;
			this->attractClip.y = 0;
		}
	}
	else if( keyStates[ GameKeys::UP ] == true || keyStates[GameKeys::LEFT] == true )
	{
		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		if( this -> passed_time >= SELECTOR_DELAY_TIME )
		{
			if( current_selection > Selection::NEWGAME )
			{
				current_selection--;
			}
			else
			{
				current_selection = ( Selection::TOTAL - 1 );
			}
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
		}
	}
	else if( current_selection == Selection::NEWGAME && keyStates[ GameKeys::SPACE ] == true )
	{
		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().set_state(Game::GStates::NEW_GAME);
		this -> passed_time = 0.0;
		this -> attractClip.y = 0;
	}

	else if( current_selection == Selection::CONTINUE && keyStates[ GameKeys::SPACE ] == true)
	{
		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().set_state( Game::GStates::CONTINUE );
		this -> passed_time = 0.0;
		this -> attractClip.y = 0;
	}

	else if( current_selection == Selection::OPTIONS && keyStates[ GameKeys::SPACE ] == true)
	{
		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().set_state( Game::GStates::OPTIONS );
		this -> passed_time = 0.0;
		this -> attractClip.y = 0;
	}

	else if( current_selection == Selection::CREDITS && keyStates[ GameKeys::SPACE ] == true )
	{
		if( shouldIgnore )
		{
			this -> passed_time = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().set_state( Game::GStates::CREDITS );
		this -> passed_time = 0.0;
		this -> attractClip.y = 0;
	}
}
