/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Level.cpp
* The levels of the game.
* Class that controll the levels of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "Level.h"
#include "Logger.h"
#include "Game.h"

/**
* The constructor.
* Initializes the attributes.
*/
Level::Level() :
	width( 0 ),
	height( 0 ),
	player( nullptr ),
	camera( nullptr ),
	playerHud( nullptr ),
	tileMap( nullptr ),
	quadTree( nullptr ),
	background( nullptr ),
	backgroundTop( nullptr ),
	checkpointsX{ 0, 0, 0, 0, 0 },
	checkpointsY{ 0, 0, 0, 0, 0 },
	checkpointsVisited{ false, false, false, false, false},
	NUMBER_OF_CHECKPOINTS( 5 )
{
	// Only serves as the initializer for the derived classes.
}

/**
* The destructor.
*/
Level::~Level()
{
	this -> player = nullptr;

	if( this -> camera != nullptr )
	{
		delete this -> camera;
		this -> camera = nullptr;
	}

	if( this -> playerHud != nullptr )
	{
		delete this -> playerHud;
		this -> playerHud = nullptr;
	}

	if( this -> tileMap != nullptr )
	{
		delete this -> tileMap;
		this -> tileMap = nullptr;
	}

	if( this -> quadTree != nullptr )
	{
		delete this -> quadTree;
		this -> quadTree = nullptr;
	}
}

/**
* Change the current checkpoint, updating for the last reached checkpoint.
* @param NUMBER_OF_CHECKPOINTS_: the number maximun of checkpoints.
* @param checkpointsX_: vector for checkpoint position in axis x of the.
* @param checkpointsY_: vector for checkpoint position in axis y of the.
*/
void Level::changeCheckpoints( int NUMBER_OF_CHECKPOINTS_, std::vector <double> checkpointsX_,
		std::vector <double> checkpointsY_ )
{
	this -> checkpointsX = checkpointsX_;
	this -> checkpointsY = checkpointsY_;
	this -> NUMBER_OF_CHECKPOINTS = NUMBER_OF_CHECKPOINTS_;
}

unsigned int Level::getWidth()
{
	return this -> width;
}

unsigned int Level::getHeight()
{
	return this -> height;
}

/**
* Sets the proprieties for the players of the game.
* @param player_: the current object player will be configured.
*/
void Level::setPlayer( Player* const player_ )
{
	this -> player = player_;

	if( this -> player != nullptr)
	{
		this -> player -> setLevelWH( this -> width, this -> height );
		addEntity( this -> player );
	}
	else
	{
		Log( WARN ) << "Setting a null player for the level!";
	}
	
}

/**
* Sets the proprieties for the cameras of the game.
* @param camera_: the current object camera will be configured.
*/
void Level::setCamera(Camera *const camera_)
{
	this -> camera = camera_;

	if( this -> camera != nullptr )
	{
		if( this -> player != nullptr)
		{
			this -> camera -> setLevelWH( this -> width, this -> height );
		}
		else
		{
			Log( WARN ) << "Shouldn't set the camera before the player, in Level!";
		}
	}
	else
	{
		Log( WARN ) << "Setting a null camera!";
	}

}

/**
* Sets the proprieties for the boss of the game.
* @param bos_: the object bos will be configured.
*/
void Level::setBoss( Boss *const boss_ ){
	this -> boss = boss_;

	if( this -> boss != nullptr )
	{
		if( this -> player != nullptr )
		{
			this -> boss -> setLevelWH( this -> width, this -> height );
		}
		else
		{
			Log( WARN ) << "Shouldn't set the boss before the player, in Level!";
		}
	}
	else
	{
		Log( WARN ) << "Setting a null boss!";
	}

}

/**
* Ends with enemies in the screen.
*/
void Level::clearEnemies()
{
	for( auto enemy : this -> enemies )
	{
		delete enemy;
		enemy = nullptr;
	}

	this -> enemies.clear();
}

/**
* Ends with documents useds in the game..
*/
void Level::clearDocuments()
{
	for( auto document : this -> documents )
	{
		delete document;
		document = nullptr;
	}

	this -> documents.clear();
}
