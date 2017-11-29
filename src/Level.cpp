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
#include <assert.h>

/**
* The constructor.
* Initializes the attributes.
*/
Level::Level() :
	width( 0 ),
	height( 0 ),
	player( nullptr ),
	camera( nullptr ),
	player_Hud( nullptr ),
	tile_map( nullptr ),
	quadTree( nullptr ),
	background( nullptr ),
	backgroud_top( nullptr ),
	checkpoints_X{ 0, 0, 0, 0, 0 },
	checkpoints_Y{ 0, 0, 0, 0, 0 },
	checkpoints_visited{ false, false, false, false, false},
	TOTAL_NUMBER_OF_CHECKPOINTS( 5 )
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
		delete_camera();

	}else if( this -> player_Hud != nullptr )
	{
		delete_player_hud();

	}else if( this -> tile_map != nullptr )
	{
		delete_tile_map();

	}else if( this -> quadTree != nullptr )
	{
		delete_quadTree();

	}else
	{
		Log( DEBUG ) << "Invalid option.";
	}
}

void Level::delete_camera()
{
	delete this -> camera; // Delete the camera.
	this -> camera = nullptr;
	assert( this -> camera == nullptr );
}

void Level::delete_player_hud()
{
	delete this -> player_Hud; // Delete the player hud.
	this -> player_Hud = nullptr;
	assert( this -> player_Hud == nullptr );
}

void Level::delete_tile_map()
{
	delete this -> tile_map; // Delete the tile map.
	this -> tile_map = nullptr;
	assert(this -> tile_map == nullptr );
}

void Level::delete_quadTree()
{
	delete this -> quadTree; // Delete the QuadTree,
	this -> quadTree = nullptr;
	assert( this -> quadTree = nullptr );
}

/**
* Change the current checkpoint, updating for the last reached checkpoint.
* @param TOTAL_NUMBER_OF_CHECKPOINTS_: the number maximun of checkpoints.
* @param checkpoints_X_: vector for checkpoint position in axis x of the.
* @param checkpoints_Y_: vector for checkpoint position in axis y of the.
*/
void Level::changeCheckpoints( int TOTAL_NUMBER_OF_CHECKPOINTS_, std::vector <double> checkpoints_X_,
		std::vector <double> checkpoints_Y_ )
{
	assert( TOTAL_NUMBER_OF_CHECKPOINTS > 0 ); // Check if the total number of checkpoints is > 0.

	Log( INFO ) << "Changing checkpoints...";

	this -> checkpoints_X = checkpoints_X_;
	this -> checkpoints_Y = checkpoints_Y_;
	this -> TOTAL_NUMBER_OF_CHECKPOINTS = TOTAL_NUMBER_OF_CHECKPOINTS_;

}

// Get the level width.
unsigned int Level::getWidth()
{
	assert( this -> width );
	return this -> width;
}

// Get the level height.
unsigned int Level::getHeight()
{
	assert( this -> height );
	return this -> height;
}

/**
* Sets the proprieties for the players of the game.
* @param player_: the current object player will be configured.
*/
void Level::set_player( Player *const player_ )
{

	assert( player_ ); // Check if the player instance is not null.

	this -> player = player_;

	if( this -> player != nullptr)
	{
		this -> player -> setLevelWH( this -> width, this -> height ); // Setting player Widht and Height.
		addEntity( this -> player );

	}else
	{
		Log( WARN ) << "Setting a null player for the level!";
	}

}

/**
* Sets the proprieties for the cameras of the game.
* @param camera_: the current object camera will be configured.
*/
void Level::set_camera( Camera *const camera_ )
{
	assert( camera_ );

	this -> camera = camera_;

	if( this -> camera != nullptr )
	{
		if( this -> player != nullptr)
		{
			this -> camera -> setLevelWH( this -> width, this -> height ); // Setting camera Widht and Height.

		}else
		{
			Log( WARN ) << "Shouldn't set the camera before the player, in Level!";
		}

	}else
	{
		Log( WARN ) << "Setting a null camera!";
	}

}

/**
* Sets the proprieties for the boss of the game.
* @param bos_: the object bos will be configured.
*/
void Level::setBoss( Boss *const BOSS ){

	assert( BOSS );

	this -> boss = BOSS;

	if( this -> boss != nullptr )
	{
		if( this -> player != nullptr )
		{
			this -> boss -> setLevelWH( this -> width, this -> height ); // Setting Boss Widht and Height.

		}else
		{
			Log( WARN ) << "Shouldn't set the boss before the player, in Level!";
		}

	}else
	{
		Log( WARN ) << "Setting a null boss!";
	}

}

/**
* Ends with enemies in the screen.
*/
void Level::clear_enemies()
{
	for( auto enemy : this -> enemies )
	{
		delete enemy;
		enemy = nullptr;
	}

	this -> enemies.clear(); // Clear all the enemies.
}

/**
* Ends with documents useds in the game..
*/
void Level::clear_documents()
{
	for( auto document : this -> documents )
	{
		delete document;
		document = nullptr;
	}

	this -> documents.clear(); // Clear all the documents.
}
