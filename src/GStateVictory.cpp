/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @GStateVictory.cpp
 * The state for the initial menu screen.
 * Game state that will contain the game over screen.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "GStateVictory.h"
#include "Game.h"
#include "Logger.h"

// Initializes all the attributes.
GStateVictory::GStateVictory () :

	victoryImage ( nullptr ),
	passedTime( 0.0 ),
	lifeTime( 0.0 )
{

}

GStateVictory::~GStateVictory ()
{

}

/**
* Loads the level.
* From the menu.lua script, loads all the necessary objects.
*/
void GStateVictory::load ()
{

	Log ( DEBUG ) << "Loading victory...";

    this -> victoryImage = Game::instance (). getResources (). get(
    	"res/images/victory.png" );

	this -> lifeTime = 5.0;

	// Changing the music.
	Game::instance (). getAudioHandler (). changeMusic( "res/audio/Tema_Victory_01.wav" );
}

/**
* Unloads everything that was loaded.
* @see GStateVictory::load
*/
void GStateVictory::unload ()
{

	Log ( DEBUG ) << "\tUnloading victory...";
	cleanEntities ();

	this -> passedTime = 0.0;
	this -> lifeTime = 0.0;

}

/**
* Updates the objects within the StateGame.
* @param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void GStateVictory::update ( const double dt_ )
{

	this -> passedTime += dt_;

	if ( this -> passedTime >= this -> lifeTime )
	{
		Game::instance (). setState ( Game::GStates::CREDITS );
		return;

	}
}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateVictory::render ()
{

	if ( this -> victoryImage != nullptr )
	{

		this -> victoryImage -> render ( 0, 0, nullptr, true );

	}else
	{
		Log ( WARN ) << "No image set for the victory screen!";
	}
}
