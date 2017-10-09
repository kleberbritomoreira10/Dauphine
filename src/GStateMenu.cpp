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
	menuImage( nullptr ),
	menuSelector( nullptr ),
	attractModeBg( nullptr ),
	attractMode( nullptr ),
	passedTime( 0.0 ),
	currentSelection( Selection::NEWGAME ),
	selectorXPositionLeft { 610, 635, 635, 645 },
	selectorYPositionLeft { 560, 625, 690, 755 },
	selectorXPositionRight { 880, 855, 855, 845 },
	selectorYPositionRight { 560, 625, 690, 755 },
	attractHeightSize( 600 ),
	attractChangeSpeed( 1 ),
	attractClip{ 0, 0, 0, this -> attractHeightSize },
	shwingAnimation( nullptr ),
	shwingIsActivated( true ),
	shwing( nullptr ),
	shwingClip { 0,0,0,0 }
{

}

/**
* The destructor.
*/
GStateMenu::~GStateMenu()
{
	if( this -> shwingAnimation != nullptr )
	{
		delete this -> shwingAnimation;
		this -> shwingAnimation = nullptr;
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

    this -> menuImage = Game::instance().getResources().get( pathTitleScreen );
    this -> menuSelector = Game::instance().getResources().get( pathCursor );
    this -> attractModeBg = Game::instance().getResources().get("res/images/title_background.png");
    this -> attractMode = Game::instance().getResources().get("res/images/attract.png");
    this -> attractClip.w = this -> attractMode -> getWidth();
    this -> shwingAnimation = new Animation( 0, 0, 795, 360, 3, false );
    this -> shwing = Game::instance().getResources().get("res/images/shwing_sheet.png");
    this -> shwingAnimation -> ANIMATION_LIMIT = 2;

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
	this -> passedTime += DELTA_TIME;

	handleSelectorMenu();

	this -> shwingAnimation -> update( this -> shwingClip, DELTA_TIME );	

	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();
	if( keyStates[ GameKeys::ESCAPE ] == true )
	{
		Game::instance().stop();
	}

	if( this -> shwingIsActivated )
	{
		 this -> shwingAnimation -> changeAnimation( 0, 0, 12, false, 2 );
		 this -> shwingIsActivated = false;
	}

}

/**
* Show the menu of the game with your properties.
*/
void GStateMenu::render()
{

	if( this -> passedTime > 10 )
	{
		this -> attractModeBg -> render( 0, 0, nullptr, true );
		this -> attractMode -> render( 0, 0, &this -> attractClip, true );
		shouldIgnore = true;
		if( this -> attractClip.y < ( int ) this -> attractMode -> getHeight() - this -> attractHeightSize )
		{
			this -> attractClip.y  += this -> attractChangeSpeed;
		}
		else
		{
			//shwing->render(340,50,&this->shwingClip);
		}
		if( this -> passedTime > 75 )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
		}
	}
	else
	{
		if( this -> menuImage != nullptr )
		{
			this -> menuImage -> render( 0, 0, nullptr, true );

			this -> menuSelector -> setWidth( 50 );

			this -> menuSelector -> render( selectorXPositionLeft[ currentSelection ],
				selectorYPositionLeft[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

			this -> menuSelector -> render( selectorXPositionRight[ currentSelection ],
				selectorYPositionRight[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );

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
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		if( this -> passedTime >= SELECTOR_DELAY_TIME )
		{
			if( currentSelection < ( Selection::TOTAL - 1 ) )
			{
				currentSelection++;
			}
			else
			{
				currentSelection = Selection::NEWGAME;
			}
			
			this->passedTime = 0.0;
			this->attractClip.y = 0;
		}
	}
	else if( keyStates[ GameKeys::UP ] == true || keyStates[GameKeys::LEFT] == true )
	{
		if( shouldIgnore )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		if( this -> passedTime >= SELECTOR_DELAY_TIME )
		{
			if( currentSelection > Selection::NEWGAME )
			{
				currentSelection--;
			}
			else
			{
				currentSelection = ( Selection::TOTAL - 1 );
			}
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
		}
	}
	else if( currentSelection == Selection::NEWGAME && keyStates[ GameKeys::SPACE ] == true )
	{
		if( shouldIgnore )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().setState(Game::GStates::NEW_GAME);
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

	else if( currentSelection == Selection::CONTINUE && keyStates[ GameKeys::SPACE ] == true)
	{
		if( shouldIgnore )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().setState( Game::GStates::CONTINUE );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

	else if( currentSelection == Selection::OPTIONS && keyStates[ GameKeys::SPACE ] == true)
	{
		if( shouldIgnore )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().setState( Game::GStates::OPTIONS );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

	else if( currentSelection == Selection::CREDITS && keyStates[ GameKeys::SPACE ] == true )
	{
		if( shouldIgnore )
		{
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

		Game::instance().setState( Game::GStates::CREDITS );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}
}
