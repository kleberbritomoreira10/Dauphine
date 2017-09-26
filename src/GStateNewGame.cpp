/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @GStateNewGame.cpp
 * The state for the New Game menu screen.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "GStateNewGame.h"
#include "LuaScript.h"
#include "Game.h"

#include "Util.h"

#include <string>

/*
The state for the New Game menu screen.
The player can start a new game by selecting a new slot to save the game */
GStateNewGame::GStateNewGame () :

	background ( nullptr ),
	selector ( nullptr ),
	passed_time ( 0.0 ),
	current_selection ( Selection::SLOT_1 ),
	selectorXPosition ( 562 ),
	selectorYPosition { 500, 610, 723 }
{

	/*
	Only 3 slots are instantiated for a new game.
	When loading the font of the text appears "Empty slot" or the level that the
	player stopped in the slot that was taken.
	*/
	this -> slot1 = new Text ( 615.0, 520.0, "res/fonts/maturasc.ttf",
		45, "Empty Slot");

	this -> slot2 = new Text ( 615.0, 630.0, "res/fonts/maturasc.ttf",
		45, "Empty Slot");

	this -> slot3 = new Text ( 615.0, 730.0, "res/fonts/maturasc.ttf",
		45, "Empty Slot");

}

// Check whether the slot has already been used or not
GStateNewGame::~GStateNewGame ()
{

	if ( this -> slot1 != nullptr )
	{

		delete this -> slot1;
		this -> slot1 = nullptr;

	}

	if ( this -> slot2 != nullptr )
	{
		delete this -> slot2;
		this -> slot2 = nullptr;
	}

	if ( this -> slot3 != nullptr )
	{
		delete this -> slot3;
		this -> slot3 = nullptr;
	}
}

/*
Loads the level.
From the menu.lua script, loads all the necessary objects.
*/
void GStateNewGame::load ()
{

	Log ( DEBUG ) << "Loading Choose Slot Screen...";

	// Start a new game on slot 1.
	if ( Game::instance (). get_saves (). is_saved ( SLOT_1 ) )
	{

		const int LEVEL_FROM_SAVE = Game::instance (). get_saves (). get_saved_level( SLOT_1 );

		const std::string CURRENT_LEVEL = "Level " +
			Util::toString ( LEVEL_FROM_SAVE );

		if ( LEVEL_FROM_SAVE == -1 )
		{

			this -> slot1 -> changeText( "Empty Slot" );

		}else
		{
			this -> slot1 -> changeText ( CURRENT_LEVEL. c_str () );
		}
	}else
	{
		this -> slot1 -> changeText ( "Empty Slot" );
	}

	// Start a new game on slot 2.
	if ( Game::instance (). get_saves (). is_saved ( SLOT_2 ) )
	{

		const int LEVEL_FROM_SAVE = Game::instance (). get_saves (). get_saved_level ( SLOT_2 );

		const std::string CURRENT_LEVEL = "Level " + Util::toString ( LEVEL_FROM_SAVE );

		if ( LEVEL_FROM_SAVE == -1 )
		{

			this -> slot2 -> changeText ( "Empty Slot" );

		}else
		{
			this -> slot2 -> changeText ( CURRENT_LEVEL.c_str () );
		}

	}else
	{
		this -> slot2 -> changeText ( "Empty Slot" );
	}

	// Start a new game on slot 3.
	if ( Game::instance (). get_saves (). is_saved ( SLOT_3 ) )
	{

		const int LEVEL_FROM_SAVE = Game::instance (). get_saves (). get_saved_level ( SLOT_3 );

		const std::string CURRENT_LEVEL = "Level " + Util::toString ( LEVEL_FROM_SAVE );

		if ( LEVEL_FROM_SAVE == -1 )
		{
			this -> slot3 -> changeText ( "Empty Slot" );
		}else
		{

			this -> slot3 -> changeText ( CURRENT_LEVEL. c_str () );
		}

	}else
	{
		this -> slot3 -> changeText ( "Empty Slot" );
	}

	//As is uses the same resources as the Continue screen...
	LuaScript luaMenu ( "lua/Continue.lua");

	const std::string PATH_BACKGROUND = luaMenu.unlua_get < std::string > (
		"continue.images.background" );

	const std::string PATH_SELECTOR = luaMenu.unlua_get < std::string > (
		"continue.images.selector" );

	this -> background = Game::instance (). get_resources (). get ( PATH_BACKGROUND );
    this -> selector = Game::instance (). get_resources (). get ( PATH_SELECTOR );

	this -> selector -> setWidth ( 410 );
	this -> selector -> setHeight ( 102 );

    this -> current_selection = Selection::SLOT_1;

    Game::instance (). get_fade (). fade_out ( 0, 0.002 );
}

// Unloads everything that was loaded.
void GStateNewGame::unload ()
{

	Log ( DEBUG ) << "\tUnloading menu...";
	clean_entities ();

}

// Updates the objects within the StateGame.
void GStateNewGame::update ( const double DELTA_TIME )
{

	this -> passed_time += DELTA_TIME;

	handleSelectorMenu ();

	std::array < bool, GameKeys::MAX > keyStates = Game::instance (). getInput ();

	if ( keyStates [ GameKeys::ESCAPE ] == true )
	{
		Game::instance ().set_state ( Game::GStates::MENU );
	}
}

/*
Renders the state.
Always renders on 0,0 position.
*/
void GStateNewGame::render ()
{

	if ( this -> background != nullptr )
	{

		this -> background -> render ( 0, 0, nullptr, true );

		this -> selector -> render ( selectorXPosition, selectorYPosition [ current_selection ],
		 nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

		this -> slot1 -> render ( 0, 0 );
		this -> slot2 -> render ( 0, 0 );
		this -> slot3 -> render ( 0, 0 );

	}else
	{
		Log ( WARN ) << "No image set to display on the menu!";
	}
}

// Handle with the input that player select on menu in the game.
void GStateNewGame::handleSelectorMenu ()
{

	std::array < bool, GameKeys::MAX > keyStates = Game::instance ().getInput ();

	const double SELECTOR_DELAY_TIME = 0.2;

	// When an attack key is pressed the game returns to the start menu.
	if ( keyStates [ GameKeys::LATTACK ] == true )
	{

		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
		{
			Game::instance (). set_state ( Game::GStates::MENU );
		}
	}

	// When the down or right key is pressed the selection goes to the next slot.
	if ( keyStates [ GameKeys::DOWN ] == true || keyStates [ GameKeys::RIGHT ]
		 == true )
	{

		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
		{

			if ( current_selection < ( Selection::TOTAL - 1 ) )
			{
				current_selection++;
			}else
			{
				current_selection = Selection::SLOT_1;
			}

			this -> passed_time = 0.0;

		}
	// When the up or left key is pressed the selection goes to the previous slot.
	} else if ( keyStates [ GameKeys::UP ] == true || keyStates [ GameKeys::LEFT ]
				 == true)
			{

		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
		{

			if ( current_selection > Selection::SLOT_1 )
			{

				current_selection--;

			}else
			{
				current_selection = ( Selection::TOTAL - 1 );
			}

			this -> passed_time = 0.0;
		}

	// When the selection is in slot 1 and the space key is pressed, a new game starts saved in slot 1.
	}else if ( current_selection == Selection::SLOT_1 && keyStates [ GameKeys::SPACE ]
			 	== true)
			{

		Game::instance (). current_slot = Selection::SLOT_1;
		Game::instance (). get_saves (). setSlot ( Selection::SLOT_1 );
		Game::instance (). get_saves (). createSave ();
		Game::instance (). transition_to = Game::GStates::LEVEL_ONE;
		Game::instance (). set_state( Game::GStates::TRANSITION );

	// When the selection is in slot 2 and the space key is pressed, a new game starts saved in slot 2.
	}else if ( current_selection == Selection::SLOT_2 && keyStates [ GameKeys::SPACE ]
				== true)
			{

		Game::instance (). current_slot = Selection::SLOT_2;
		Game::instance (). get_saves (). setSlot ( Selection::SLOT_2 );
		Game::instance (). get_saves (). createSave ();
		Game::instance (). transition_to = Game::GStates::LEVEL_ONE; //should be level one, two is here for testing purposes
		Game::instance (). set_state ( Game::GStates::TRANSITION );

	// When the selection is in slot 3 and the space key is pressed, a new game starts saved in slot 3.
	}else if ( current_selection == Selection::SLOT_3 && keyStates[GameKeys::SPACE]
				== true)
			{

		Game::instance (). current_slot = Selection::SLOT_3;
		Game::instance (). get_saves (). setSlot ( Selection::SLOT_3 );
		Game::instance (). get_saves (). createSave ();
		Game::instance (). transition_to = Game::GStates::LEVEL_BOSS; //should be level one, boss is here for testing purposes
		Game::instance (). set_state ( Game::GStates::TRANSITION );
	}

}
