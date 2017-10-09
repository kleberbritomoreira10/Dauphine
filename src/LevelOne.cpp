/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LevelOne.cpp
 * The first level of the game.
 * Derived from Level class
 * License: Copyright (C) 2014 Alke Games.
 */

#include "LevelOne.h"
#include "Game.h"
#include "GameSave.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"

double ok = 0; // iterator that determines when to change the state to game over

// Quantity of items that can be caught in phase 1.
LevelOne::LevelOne ():
	Level(),
	items {{ 59 * 64, 114 * 64, 0, 0 },{ 1750, 1750, 0, 0 }},
	caught_items { false, false, true, true }
{
	this -> changeCheckpoints ( 2, { 4000, 7500 }, { 1600, 1600 });
}

LevelOne::~LevelOne ()
{

}

/**
* Loads the level.
* From the Level1.lua script, loads all the necessary objects.
*/
void LevelOne::load ()
{
	Log ( DEBUG ) << "Loading level 1...";

	// Loading the tile/tilemap.
	this -> tile_map = new TileMap ( "res/maps/level1.tmx" );

	// Setting the level width/height.
	this -> width = this -> tile_map -> getMapWidth ();

	this -> height = this -> tile_map -> getMapHeight ();

	SDL_Rect bounds = { 0, 0, ( int )this -> width, ( int )this -> height };

	this -> quadTree = new QuadTree ( 0, bounds );

	this -> background = Game::instance (). getResources (). get (
	 "res/images/lv1_background_parallax.png" );

	this -> backgroundTop = Game::instance (). getResources (). get (
	 "res/images/lv1_parallax_top.png" );

	 // Load the number of checkpoints achieved
	for ( int i = 0; i < this -> NUMBER_OF_CHECKPOINTS; ++i )
	{

		this -> checkpoints.push_back ( Game::instance (). getResources (). get (
		 "res/images/checkpoint.png" ));

	}

	// Getting information from lua script.
	LuaScript luaLevel1 ( "lua/Level1.lua" );

	const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get <std::string> (
		"level.player.spriteSheet" );

	const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get <std::string> (
		"level.audio.background" );

	const std::string PATH_ENEMY = luaLevel1.unlua_get <std::string> ( "level.enemy" );


	// Changing the music.
	Game::instance (). get_audio_handler (). change_music ( PATH_BACKGROUND_AUDIO );

	// Loading the player and the camera.

	Player *level_player = nullptr;

	if ( Game::instance (). get_saves (). is_saved ( Game::instance (). current_slot )
	 && Game::instance (). get_saves (). get_saved_level ( Game::instance (). current_slot ) == 1 )
	{

		double saved_x_position = 0.0;
		double saved_y_position = 0.0;

		Game::instance (). get_saves (). get_player_position ( saved_x_position, saved_y_position,
			Game::instance ().current_slot );

		level_player = new Player ( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
	}else
	{
		level_player = new Player ( this -> tile_map -> get_initial_x (), this -> tile_map ->
			get_initial_y (), PATH_PLAYER_SPRITE_SHEET );
	}

	Camera *level_camera = new Camera ( level_player );

	// Loading the refill of potion.
	this -> image = Game::instance (). getResources (). get( "res/images/potion.png" );

	this -> player_Hud = new PlayerHUD( level_player );

	// Load all the enemies from the tile_map.
	for ( unsigned int i = 0; i < this -> tile_map -> getEnemiesX (). size (); i++ )
	{

		Enemy *enemy = new Enemy ( this -> tile_map -> getEnemiesX (). at( i ),
			this -> tile_map -> getEnemiesY (). at( i ), PATH_ENEMY,
			this -> tile_map -> getEnemiesPatrol (). at( i ), 0.0 );

		if ( Game::instance (). get_saves (). is_saved ( Game::instance (). current_slot ) )
		{

			if ( Game::instance (). get_saves (). isEnemyDead ( i, Game::instance (). current_slot )
				&& Game::instance (). get_saves (). get_saved_level ( Game::instance (). current_slot )
				== 1 )
			{

				enemy -> setDead ( true );

			}
		}

		enemy -> setLevelWH ( this -> width, this -> height );
		this -> enemies. push_back ( enemy );

	}

	// Finally, setting the player and the camera.
	setPlayer ( level_player );

	Enemy::pLife = this -> player -> life;

	setCamera ( level_camera );

	Game::instance (). getFade (). fadeOut ( 0, 0.002 );
}

// Unloads everything that was loaded.

void LevelOne::unload ()
{
	Log ( DEBUG ) << "\tUnloading level 1...";

	cleanEntities ();
	clearEnemies ();
	clearDocuments ();

	for ( int i = 0; i < NUMBER_ITEMS; ++i )
	{

		caught_items [ i ] = false;
	}
}

/**
* Updates the objects within the Level.
* @param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void LevelOne::update ( const double dt_ )
{

	// Populating the QuadTree.
	this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

	// Updating the entities, using the QuadTree.
	std::vector < CollisionRect > returnObjects;

	for ( auto entity : this -> entities )
	{

		returnObjects. clear ();

		this -> quadTree -> retrieve ( returnObjects, entity -> getBoundingBox () );

		entity -> setCollisionRects ( returnObjects );

		entity -> update( dt_ );

	}

	// Updating the enemies.
	for ( auto enemy : this -> enemies )
	{

		returnObjects. clear ();

		this -> quadTree -> retrieve ( returnObjects, enemy -> getBoundingBox () );

		enemy -> setCollisionRects ( returnObjects );

		enemy -> update( dt_ );

	}

	// Set to GameOver if the player is dead.
	if ( this -> player -> isDead () )
	{

		this -> player -> changeState ( Player::PStates::DEAD );

		ok = ok + dt_;

		if( ok > 3 )
		{

			Game::instance (). setState ( Game::GStates::GAMEOVER );

		}

		return;

	}

	// Updating the potions.
	for( auto potion : this -> player -> potions )
	{

		returnObjects. clear ();

		this -> quadTree -> retrieve ( returnObjects, potion -> getBoundingBox () );

		potion -> setCollisionRects ( returnObjects );

	}

	/// @todo Maybe refactor this static Enemy::px, Enemy::py.
	// Updating player info for the enemies.
	Enemy::px = this -> player -> x;
	Enemy::py = this -> player -> y;
	Enemy::pVulnerable = this -> player -> isVulnerable;

	for ( int i = 0; i < NUMBER_ITEMS; ++i )
	{

		if ( Collision::rectsCollided ( this -> player -> getBoundingBox (),
			{ items [ 0 ] [ i ], items [ 1 ] [ i ], 192, 192 } ) && caught_items [ i ] == false )
		{

			this -> player -> addPotions ( 3 );

			caught_items [ i ] = true;

		}
	}

 	if ( this -> player -> life != Enemy::pLife )
 	{

		if ( this -> player -> isVulnerable )
		{

			this -> player -> life--;
			Enemy::pLife = this -> player -> life;
			this -> player -> changeState ( Player::PStates::HITED );
			this -> player -> isVulnerable = false;
		}else
		{
			//Do nothing.
		}
 	}

	// Updating the HUD.
	this -> player_Hud -> update ();

	// Updating the camera.
	this -> camera -> update ();

	// Set next level if end is reached.
	if ( this -> player -> reachedLevelEnd )
	{

		Game::instance (). transitionTo = Game::GStates::LEVEL_TWO;
		Game::instance (). setState ( Game::GStates::TRANSITION );
		return;

	}

	// Updating the potion/enemy collision.
	for ( auto potion : this -> player -> potions )
	{
		for ( auto enemy : this -> enemies )
		{

			if ( Collision::rectsCollided ( potion -> getBoundingBox (),
				enemy -> getBoundingBox () ) )
			{

				if ( potion -> activated )
				{

					if ( enemy -> life > 0 && this -> player -> canAttack )
					{

						enemy -> life -= 100;
						potion -> activated = false;

					}
					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if ( enemy -> life <= 0 )
					{

						enemy -> changeState ( Enemy::EStates::DEAD );

					}
				}
			}
		}
	}

	// Updating the player attack/enemy collision.
	for ( auto enemy : this -> enemies )
	{

		if ( Collision::rectsCollided ( this -> player -> getBoundingBox (),
			enemy -> getBoundingBox () ) )
		{

			if ( this -> player -> isRight != enemy -> isRight )
			{

				if ( this -> player -> isCurrentState ( Player::PStates::ATTACK ) ||
					this -> player -> isCurrentState ( Player::PStates::ATTACKMOVING ) )
				{

					if ( enemy -> life > 0 && this -> player -> canAttack )
					{

						enemy -> life -= this -> player -> attackStrength;
						this -> player -> canAttack = false;

					}

					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if ( enemy -> life <= 0 )
					{

						enemy -> changeState ( Enemy::EStates::DEAD );

					}
				}
			}
		}
	}

	//Saving the game state
	for ( int j = 0; j < this -> NUMBER_OF_CHECKPOINTS; ++j )
	{

		if ( !this -> checkpointsVisited [ j ] && this -> player -> getBoundingBox (). x
			>= checkpointsX [ j ] && this -> player -> getBoundingBox (). x <= checkpointsX[j]
		    + 100 && this -> player -> getBoundingBox (). y >= checkpointsY [ j ]
		    && this -> player -> getBoundingBox (). y <= checkpointsY [ j ] + 200 )
		{

			this -> checkpoints [ j ] = Game::instance (). getResources (). get (
				"res/images/checkpoint_visited.png" );

			Game::instance (). get_saves ().saveLevel ( 1, this -> player, this -> enemies,
				Game::instance (). current_slot );

			this -> checkpointsVisited [ j ] = true;

		}
	}

	// Checks the condition of the document to render
	for ( auto document : this -> documents )
	{

		if ( Collision::rectsCollided ( this -> player -> getBoundingBox (),
			document -> getBoundingBox () ) )
		{
			document -> shouldRender = true;
		}else
		{
			document -> shouldRender = false;
		}
	}
}

/*
Renders the level.
Always renders on 0,0 position.
*/
void LevelOne::render ()
{

	const int cameraX = this -> camera -> getClip (). x;
	const int cameraY = this -> camera -> getClip (). y;

	this -> background -> render ( 0, 480 - cameraY );
	this -> backgroundTop -> render( -cameraX / 10, 1165 - cameraY );

	// Render the tiles in the TileMap.
	this -> tile_map -> render ( cameraX, cameraY );

	for ( int j = 0; j < this -> NUMBER_OF_CHECKPOINTS; ++j )
	{
		this -> checkpoints [ j ] -> render ( this -> checkpointsX [ j ] - cameraX,
			this -> checkpointsY [ j ] - cameraY );
	}

	this -> player_Hud -> render ();

	for ( auto enemy : this -> enemies )
	{
		enemy -> render ( cameraX, cameraY );
	}

	// Render all the entities in the list.
	for ( auto entity : this -> entities )
	{
        entity -> render ( cameraX, cameraY );
	}

	for ( unsigned int i = 0; i < NUMBER_ITEMS; i++ )
	{

		if ( this -> image != nullptr && caught_items [ i ] == false )
		{

			this -> image -> Sprite::render ( ( items [ 0 ] [ i ] + 60 ) - cameraX,
				( ( items [ 1 ] [ i ] ) - cameraY ) );

		}
	}

	// Render the document text
	for ( auto document : this -> documents )
	{

		document -> render ( cameraX, cameraY );

		if ( document -> shouldRender )
		{
			document -> renderDocumentText ();
		}
	}

}
