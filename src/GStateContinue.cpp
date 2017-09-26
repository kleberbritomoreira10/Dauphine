/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @GStateContinue.cpp
* The state for the Continue menu screen.
*/

#include "GStateContinue.h"
#include "LuaScript.h"
#include "Game.h"
#include "Util.h"
#include <string>

/**
* The constructor.
* Initializes all the attributes.
*/
GStateContinue::GStateContinue() :
	background ( nullptr ),
	selector ( nullptr ),
	passed_time( 0.0 ),
	current_selection ( Selection::SLOT_1 ),
	selectorXPosition ( 562 ),
	selectorYPosition { 500, 610, 723 }
{
	this -> slot1 = new Text ( 615.0, 520.0, "res/fonts/maturasc.ttf", 45, "Empty Slot" );
	this -> slot2 = new Text ( 615.0, 630.0, "res/fonts/maturasc.ttf", 45, "Empty Slot" );
	this -> slot3 = new Text ( 615.0, 730.0, "res/fonts/maturasc.ttf", 45, "Empty Slot" );
}

/**
* The destructor.
*/
GStateContinue::~GStateContinue ()
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

/**
* Loads the level.
* From the menu.lua script, loads all the necessary objects.
*/
void GStateContinue::load ()
{
	Log(DEBUG) << "Loading Continue Screen...";
	
	if ( Game::instance().get_saves().is_saved( SLOT_1 ) )
  {
		//Load level 2 if it was saved in slot 1
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_1 );
		
		//Assign level saved in slot 1 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );
		
		if ( LEVEL_FROM_SAVE == -1 )
    {
			this -> slot1 -> changeText( "Empty Slot" );
    }

		else
    {
			this -> slot1 -> changeText( CURRENT_LEVEL.c_str() );
    }
	}

	else
  {
		this -> slot1 -> changeText( "Empty Slot" );
	}

	if ( Game::instance().get_saves().is_saved( SLOT_2 ) )
  {
		//Load level 2 if it was saved in slot 2
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_2 );
		
		//Assign level saved in slot 2 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );
		
		if ( LEVEL_FROM_SAVE == -1 )
    {
			this -> slot2 -> changeText( "Empty Slot" );
    }

		else
    {
			this -> slot2 -> changeText( CURRENT_LEVEL.c_str() );
    }

	}
	else
  {
		this -> slot2 -> changeText( "Empty Slot" );
	}


	if ( Game::instance().get_saves().is_saved( SLOT_3 ) )
  {
		//Load level 2 if it was saved in slot 3
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_3 );

		//Assign level saved in slot 3 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );
			
		if ( LEVEL_FROM_SAVE == -1 )
    {

			this -> slot3 -> changeText( "Empty Slot" );
    }
		
    else
    {
			this -> slot3 -> changeText( CURRENT_LEVEL.c_str() );
    }
	}
	else
  {
		this -> slot3 -> changeText( "Empty Slot" );
	}

	LuaScript luaMenu( "lua/Continue.lua" );

	//Declaring constant as path to receive background image
	const std::string PATH_BACKGROUND = luaMenu.unlua_get<std::string>( "continue.images.background" );

	//Declaring constant as path to receive background image
	const std::string PATH_SELECTOR = luaMenu.unlua_get<std::string>( "continue.images.selector" );

	this -> background = Game::instance().get_resources().get( PATH_BACKGROUND );
	this -> selector = Game::instance().get_resources().get( PATH_SELECTOR );
	this -> selector -> setWidth ( 410 );
	this -> selector -> setHeight ( 102 );
	this -> current_selection = Selection::SLOT_1;

	Game::instance().get_fade().fade_out ( 0, 0.002);
}

/**
* Updates the objects within the StateGame.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void GStateContinue::unload ()
{
	Log(DEBUG) << "\tUnloading menu...";
	clean_entities();
}

/**
* Unloads everything that was loaded.
* @see GStateContinue::load
*/
void GStateContinue::update ( const double DELTA_TIME )
{
	this -> passed_time += DELTA_TIME;

	handleSelectorMenu ();

	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();
	if ( keyStates [ GameKeys::ESCAPE ] == true )
  {
		Game::instance().set_state( Game::GStates::MENU );
	}

}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateContinue::render(){

	if ( this -> background != nullptr )
  {
		this -> background -> render( 0, 0, nullptr, true );
		this -> selector -> render( selectorXPosition, selectorYPosition [ current_selection ], 
      nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );
	
		this -> slot1 -> render( 0, 0 );
		this -> slot2 -> render( 0, 0 );
		this -> slot3 -> render( 0, 0 );
	}

	else
  {
		Log(WARN) << "No image set to display on the menu!";
	}
}

/**
* Treats the slots selection menu for the game
*/
void GStateContinue::handleSelectorMenu ()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	//Declaring constant to assign time delay.
	const double SELECTOR_DELAY_TIME = 0.2;

	if ( keyStates [ GameKeys::LATTACK ] == true )
  {
		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
    {
			Game::instance().set_state( Game::GStates::MENU );
		}
	}

	if ( keyStates [ GameKeys::DOWN ] == true || keyStates [ GameKeys::RIGHT ] == true )
  {
		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
    {
			if ( current_selection < (Selection::TOTAL - 1) )
      {
				current_selection++;
			}

			else
      {
				current_selection = Selection::SLOT_1;
			}

			this -> passed_time = 0.0;
		}
	}

	else if ( keyStates [ GameKeys::UP ] == true || keyStates [ GameKeys::LEFT ] == true )
  {
		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
    {
			if ( current_selection > Selection::SLOT_1 )
      {
				current_selection--;
			}

			else
      {
				current_selection = ( Selection::TOTAL - 1 );
			}
			this -> passed_time = 0.0;
		}
	}

	else if ( current_selection == Selection::SLOT_1 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_1;

		switch ( Game::instance().get_saves().get_saved_level(Selection::SLOT_1) )
    {
			Game::instance().current_slot = Selection::SLOT_1;
			case 1:
				Game::instance().transition_to = Game::GStates::LEVEL_ONE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 2:
				Game::instance().transition_to = Game::GStates::LEVEL_TWO;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 3:
				Game::instance().transition_to = Game::GStates::LEVEL_THREE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 4:
				Game::instance().transition_to = Game::GStates::LEVEL_FOUR;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 5:
				Game::instance().transition_to = Game::GStates::LEVEL_FIVE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 6:
				Game::instance().transition_to = Game::GStates::LEVEL_BOSS;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	default:
				break;
		}

	}

	else if ( current_selection == Selection::SLOT_2 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_2;

		switch( Game::instance().get_saves().get_saved_level( Selection::SLOT_2 ) ){
			Game::instance().current_slot = Selection::SLOT_2;
			case 1:
				Game::instance().transition_to = Game::GStates::LEVEL_ONE;
				Game::instance().set_state( Game::instance().transition_to );
				break;

			case 2:
				Game::instance().transition_to = Game::GStates::LEVEL_TWO;
				Game::instance().set_state( Game::instance().transition_to );
				break;
			
      case 3:
				Game::instance().transition_to = Game::GStates::LEVEL_THREE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
			
      case 4:
				Game::instance().transition_to = Game::GStates::LEVEL_FOUR;
				Game::instance().set_state( Game::instance().transition_to );
				break;
			
      case 5:
				//CHANGE THIS AFTER CREATING FIFTH LEVEL
				Game::instance().transition_to = Game::GStates::LEVEL_FIVE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
			
      case 6:
				Game::instance().transition_to = Game::GStates::LEVEL_BOSS;
				Game::instance().set_state( Game::instance().transition_to );
				break;
			
      default:
				break;
		}

	}

	else if ( current_selection == Selection::SLOT_3 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_3;

		switch ( Game::instance().get_saves().get_saved_level(Selection::SLOT_3) )
    {
			Game::instance().current_slot = Selection::SLOT_3;
			case 1:
				Game::instance().transition_to = Game::GStates::LEVEL_ONE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 2:
				Game::instance().transition_to = Game::GStates::LEVEL_TWO;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 3:
				Game::instance().transition_to = Game::GStates::LEVEL_THREE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 4:
				Game::instance().transition_to = Game::GStates::LEVEL_FOUR;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 5:
				//CHANGE THIS AFTER CREATING FIFTH LEVEL
				Game::instance().transition_to = Game::GStates::LEVEL_FIVE;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	case 6:
				Game::instance().transition_to = Game::GStates::LEVEL_BOSS;
				Game::instance().set_state( Game::instance().transition_to );
				break;
		
    	default:
				break;
		}

	}

}
