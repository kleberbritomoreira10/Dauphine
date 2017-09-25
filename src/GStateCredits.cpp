/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @GStateCredits.cpp
* The state for the initial menu screen.
* Game state that will contain the game over screen.
* License: Copyright (C) 2014 Alke Games.*/

#include "GStateCredits.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Game.h"

#include <string>

/**
* The constructor.
* Initializes all the attributes.
*/
GStateCredits::GStateCredits() :

	creditsImage( nullptr ),
	creditsHeightSize( 600 ),
	creditsChangeSpeed( 2 ),
	creditsClip{ 0, 0, 0, this ->  creditsHeightSize }
{

}

/**
* The destructor.
*/
GStateCredits::~GStateCredits(){

}

/**
* Updates the objects within the StateGame.
* @param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void GStateCredits::update( const double dt_ )
{
	( void( dt_ ) ); //unused

	std::array< bool, GameKeys::MAX > keyStates = Game::instance().getInput();

	// Setting the menu state.
	if ( keyStates[ GameKeys::LATTACK ] == true )
	{
		Game::instance().setState( Game::GStates::MENU );
	}
}

/**
* Loads the level.
* From the menu.lua script, loads all the necessary objects.
*/
void GStateCredits::load()
{
	Log( DEBUG ) << "Loading credits...";

	// Loading lua modules.
	LuaScript luaCredits( "lua/Credits.lua" );
	const std::string pathCredits = luaCredits.unlua_get<std::string>( "credits.images.dummy" );

	// Loading credits image.
    this ->  creditsImage = Game::instance().getResources().get( pathCredits );
    this ->  creditsClip.w = this ->  creditsImage ->  getWidth();
}

/**
* Unloads everything that was loaded.
* @see GStateCredits::load
*/
void GStateCredits::unload()
{
	Log( DEBUG ) << "\tUnloading credits...";
	this ->  creditsClip.y = 0;
	cleanEntities();
}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateCredits::render()
{
	// Rendering the credits with the image and speed determined.
	if ( this ->  creditsImage != nullptr )
	{
		if ( this ->  creditsClip.y < ( int )this ->  creditsImage ->  getHeight() - this ->  creditsHeightSize )
		{
			this ->  creditsClip.y += this ->  creditsChangeSpeed;
		}
		this ->  creditsImage ->  render( 0, 0, &this ->  creditsClip, true );
	} else
	{
		Log( WARN ) << "No image set for the options screen!";
	}
}
