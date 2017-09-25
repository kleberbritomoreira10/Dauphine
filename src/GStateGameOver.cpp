/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @AudioHandler.cpp
* The audio handler.
* The state for the initial menu screen.
* Game state that will contain the game over screen.
* License: Copyright (C) 2014 Alke Games.*/


#include "GStateGameOver.h"
#include "LuaScript.h"
#include "Game.h"

#include <string>

/**
* The constructor.
* Initializes all the attributes.
*/
GStateGameOver::GStateGameOver() :
	gameOverImage( nullptr ),
	passedTime( 0.0 ),
	lifeTime( 0.0 )
{

}

/**
* The destructor.
*/
GStateGameOver::~GStateGameOver()
{

}

/**
* Loads the level.
* From the menu.lua script, loads all the necessary objects.
*/
void GStateGameOver::load()
{
	Log( DEBUG ) << "Loading Game Over...";

	// Loading game over images from lua.
	LuaScript luaGameOver( "lua/GameOver.lua" );
	const std::string pathGameOver = luaGameOver.unlua_get<std::string>( "gameOver.images.gameOver" );
	const double luaLifeTime = luaGameOver.unlua_get<double>( "gameOver.lifeTime" );

	// Getting game over images resources.
    this -> gameOverImage = Game::instance().getResources().get( pathGameOver );
	this -> lifeTime = luaLifeTime;

	// Changing the music.
	Game::instance().getAudioHandler().changeMusic( "res/audio/Game_Over.mid" );
}

/**
* Unloads everything that was loaded.
* @see GStateGameOver::load
*/
void GStateGameOver::unload()
{
	Log( DEBUG ) << "\tUnloading Game Over...";
	cleanEntities();

	this -> passedTime = 0.0;
	this -> lifeTime = 0.0;

	Game::instance().getAudioHandler().stopMusic();
}

/**
* Updates the objects within the StateGame.
* @param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void GStateGameOver::update( const double dt_ )
{
	this -> passedTime += dt_;

	std::array< bool, GameKeys::MAX > keyStates = Game::instance().getInput();

	// Setting menu state when getting space or lattack input.
	if ( keyStates[ GameKeys::SPACE ] || keyStates[ GameKeys::LATTACK ] )
	{
		Game::instance().setState( Game::GStates::MENU );
		return;
	}

	// Setting menu state when player is dead.
	if ( this -> passedTime >= this -> lifeTime )
	{
		Game::instance().setState( Game::GStates::MENU );
		return;
	}
}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateGameOver::render()
{
	// Rendering game over image.
	if ( this -> gameOverImage != nullptr )
	{
		this -> gameOverImage -> render( 0, 0, nullptr, true );
	} else
	{
		Log( WARN ) << "No image set for the game over screen!";
	}
}
