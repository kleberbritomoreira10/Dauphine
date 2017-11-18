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
#include <assert.h>
#include <cstddef>

#define PATH "res/fonts/maturasc.ttf"
#define TEXT "Empty Slot"
#define SIZE 45
#define TEXT_X_POSITION 615.0
#define TEXT_Y_POSITION_SLOT1 520.0
#define TEXT_Y_POSITION_SLOT2 630.0
#define TEXT_Y_POSITION_SLOT3 730.0
#define WIDTH 410
#define HEIGHT 102
#define CAMERA_X 0
#define CAMERA_Y 0
#define INITIAL_TIME 0.0
#define PERCENTAGE 0
#define TIME 0.002

/**
* The constructor.
* Initializes all the attributes.
*/
GStateContinue::GStateContinue() : background ( nullptr ), selector ( nullptr ), passed_time( INITIAL_TIME ), 
  current_selection ( Selection::SLOT_1 ), selectorXPosition ( 562 ), selectorYPosition { 500, 610, 723 }
{	
	assert( SLOT_1 >= 1 || SLOT_1 <= 6 );
	this -> slot1 = new Text ( TEXT_X_POSITION, TEXT_Y_POSITION_SLOT1, PATH, SIZE, TEXT );
	this -> slot2 = new Text ( TEXT_X_POSITION, TEXT_Y_POSITION_SLOT2, PATH, SIZE, TEXT );
	this -> slot3 = new Text ( TEXT_X_POSITION, TEXT_Y_POSITION_SLOT3, PATH, SIZE, TEXT );
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
	} else
	  {
      Log(INFO) << "Slot 1 is null"; 
	  }

	if ( this -> slot2 != nullptr )
  {
		delete this -> slot2;
		this -> slot2 = nullptr;
	} else
	  {
      Log(INFO) << "Slot 2 is null"; 
	  }

	if ( this -> slot3 != nullptr )
  {
		delete this -> slot3;
		this -> slot3 = nullptr;
	} else
	  {
      Log(INFO) << "Slot 3 is null"; 
	  }
}

/**
* Loads the level.
* From the menu.lua script, loads all the necessary objects.
*/
void GStateContinue::load ()
{	
	Log(DEBUG) << "Loading Continue Screen..."; 

	continue_slot1();

	continue_slot2();

	continue_slot3();	

	load_lua_script();	

	Game::instance().get_fade().fade_out ( PERCENTAGE, TIME);
}

void GStateContinue::load_lua_script()
{
  LuaScript luaMenu( "lua/Continue.lua" );

	//Declaring constant as path to receive background image
	const std::string PATH_BACKGROUND = luaMenu.unlua_get<std::string>( "continue.images.background" );

	this -> background = Game::instance().getResources().get( PATH_BACKGROUND );

	//Declaring constant as path to receive background image
	const std::string PATH_SELECTOR = luaMenu.unlua_get<std::string>( "continue.images.selector" );
	this -> selector = Game::instance().getResources().get( PATH_SELECTOR );
	
	this -> selector -> setWidth ( WIDTH );
	this -> selector -> setHeight ( HEIGHT );
	this -> current_selection = Selection::SLOT_1;
}

void GStateContinue::continue_slot1()
{
if ( Game::instance().get_saves().is_saved( SLOT_1 ) )
  {	
  	assert( SLOT_1 >= 1 || SLOT_1 <= 6);
		//Load level 2 if it was saved in slot 1
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_1 );
		
		//Assign level saved in slot 1 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );
		
		if ( LEVEL_FROM_SAVE == -1 )
    {
			this -> slot1 -> changeText( TEXT );
    } else
      {
			  this -> slot1 -> changeText( CURRENT_LEVEL.c_str() );
      }
	} else
    {
		  this -> slot1 -> changeText( TEXT );
	  }
}

void GStateContinue::continue_slot2()
{
  if ( Game::instance().get_saves().is_saved( SLOT_2 ) )
  {
  	assert( SLOT_2 >= 1 || SLOT_2 <= 6);
		//Load level 2 if it was saved in slot 2
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_2 );
		
		//Assign level saved in slot 2 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );

		if ( LEVEL_FROM_SAVE == -1 )
    {
			this -> slot2 -> changeText( TEXT );
    } else
      {
			  this -> slot2 -> changeText( CURRENT_LEVEL.c_str() );
      }

	} else
    {
		  this -> slot2 -> changeText( TEXT );
	  }
}

void GStateContinue::continue_slot3()
{
if ( Game::instance().get_saves().is_saved( SLOT_3 ) )
  {	
  	assert( SLOT_3 >= 1 || SLOT_3 <= 6);
		//Load level 2 if it was saved in slot 3
		const int LEVEL_FROM_SAVE = Game::instance().get_saves().get_saved_level( SLOT_3 );

		//Assign level saved in slot 3 to the current level
		const std::string CURRENT_LEVEL = "Level " + Util::toString( LEVEL_FROM_SAVE );
			
		if ( LEVEL_FROM_SAVE == -1 )
    {
			this -> slot3 -> changeText( TEXT );
    } else
      {
			  this -> slot3 -> changeText( CURRENT_LEVEL.c_str() );
      }
	} else
    {
		  this -> slot3 -> changeText( TEXT );
	  }
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
	assert( DELTA_TIME >= 0 );
	this -> passed_time += DELTA_TIME;

	handleSelectorMenu ();

	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();
	if ( keyStates [ GameKeys::ESCAPE ] == true )
  {
		Game::instance().setState( Game::GStates::MENU );
	} else
	  {
      //Nothing to do 
	  }
}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateContinue::render()
{

	if ( this -> background != nullptr )
  {
		this -> background -> render( CAMERA_X, CAMERA_Y, nullptr, true );
		this -> selector -> render( selectorXPosition, selectorYPosition [ current_selection ], 
      nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );
		this -> slot1 -> render( CAMERA_X, CAMERA_Y );
		this -> slot2 -> render( CAMERA_X, CAMERA_Y );
		this -> slot3 -> render( CAMERA_X, CAMERA_Y );
	} else
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
			Game::instance().setState( Game::GStates::MENU );
		} else
	    {
        //Nothing to do 
	    }
	} else
	  {
      //Nothing to do 
	  }

	if ( keyStates [ GameKeys::DOWN ] == true || keyStates [ GameKeys::RIGHT ] == true )
  {
		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
    {
			if ( current_selection < ( Selection::TOTAL - 1 ) )
      {
				current_selection++;
			} else
        {
				  current_selection = Selection::SLOT_1;
			  }

			this -> passed_time = 0.0;
		} else
	    {
        //Nothing to do 
	    }
	}

	if ( keyStates [ GameKeys::UP ] == true || keyStates [ GameKeys::LEFT ] == true )
  {
		if ( this -> passed_time >= SELECTOR_DELAY_TIME )
    {
			if ( current_selection > Selection::SLOT_1 )
      {
				current_selection--;
			} else
        {
				  current_selection = ( Selection::TOTAL - 1 );
			  }
			this -> passed_time = 0.0;
		} else
	    {
        //Nothing to do 
	    }
	} else
	  {
      //Nothing to do 
	  }

	if ( current_selection == Selection::SLOT_1 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_1;

		switch ( Game::instance().get_saves().get_saved_level(Selection::SLOT_1) )
    {
			Game::instance().current_slot = Selection::SLOT_1;
			case 1:
				Game::instance().transitionTo = Game::GStates::LEVEL_ONE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 2:
				Game::instance().transitionTo = Game::GStates::LEVEL_TWO;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 3:
				Game::instance().transitionTo = Game::GStates::LEVEL_THREE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 4:
				Game::instance().transitionTo = Game::GStates::LEVEL_FOUR;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 5:
				Game::instance().transitionTo = Game::GStates::LEVEL_FIVE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 6:
				Game::instance().transitionTo = Game::GStates::LEVEL_BOSS;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	default:
				break;
		}
	} else
	  {
      //Log(DEBUG) << "None level loaded"; 
	  }

	if ( current_selection == Selection::SLOT_2 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_2;

		switch( Game::instance().get_saves().get_saved_level( Selection::SLOT_2 ) )
		{
			Game::instance().current_slot = Selection::SLOT_2;
			case 1:
				Game::instance().transitionTo = Game::GStates::LEVEL_ONE;
				Game::instance().setState( Game::instance().transitionTo );
				break;

			case 2:
				Game::instance().transitionTo = Game::GStates::LEVEL_TWO;
				Game::instance().setState( Game::instance().transitionTo );
				break;
			
      case 3:
				Game::instance().transitionTo = Game::GStates::LEVEL_THREE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
			
      case 4:
				Game::instance().transitionTo = Game::GStates::LEVEL_FOUR;
				Game::instance().setState( Game::instance().transitionTo );
				break;
			
      case 5:
				//CHANGE THIS AFTER CREATING FIFTH LEVEL
				Game::instance().transitionTo = Game::GStates::LEVEL_FIVE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
			
      case 6:
				Game::instance().transitionTo = Game::GStates::LEVEL_BOSS;
				Game::instance().setState( Game::instance().transitionTo );
				break;
			
      default:
				break;
		} 
	} else
	  {
      //Log(DEBUG) << "None level loaded"; 
	  }

	if ( current_selection == Selection::SLOT_3 && keyStates [ GameKeys::SPACE ] == true )
  {
		Game::instance().current_slot = SLOT_3;

		switch ( Game::instance().get_saves().get_saved_level(Selection::SLOT_3) )
    {
			Game::instance().current_slot = Selection::SLOT_3;
			case 1:
				Game::instance().transitionTo = Game::GStates::LEVEL_ONE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 2:
				Game::instance().transitionTo = Game::GStates::LEVEL_TWO;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 3:
				Game::instance().transitionTo = Game::GStates::LEVEL_THREE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 4:
				Game::instance().transitionTo = Game::GStates::LEVEL_FOUR;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 5:
				//CHANGE THIS AFTER CREATING FIFTH LEVEL
				Game::instance().transitionTo = Game::GStates::LEVEL_FIVE;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	case 6:
				Game::instance().transitionTo = Game::GStates::LEVEL_BOSS;
				Game::instance().setState( Game::instance().transitionTo );
				break;
		
    	default:
				break;
		}

	} else
	  {
      //Log(DEBUG) << "None level loaded"; 
	  }
}
