/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @LevelFive.cpp
* The first level of the game.
* Derived from Level class.
* License: Copyright (C) 2014 Alke Games.*/


#include "LevelFive.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"
#include "Document.h"

#define MAX_NUMBER_OF_SLOTS 5

/**
* The constructor.
* @see Level::Level()
*/
LevelFive::LevelFive() :
  Level(),
	items{ { 207, 11261,6800, 10000 },{ 5600, 2050,5850, 2712 } },
	caught_items{ false,false,false,true }
{
	this -> changeCheckpoints( 2, { 590,5000 }, { 2700,630 } );
}

LevelFive::~LevelFive()
{

}

/**
* Loads the level.
* From the Level1.lua script, loads all the necessary objects.
*/
void LevelFive::load()
{
	// Changing the music.
	Game::instance().get_audio_handler().change_music( "res/audio/lv5.wav" );

	// Loading the tile/tilemap.
	this -> tile_map = new TileMap( "res/maps/level5.tmx" );

	// Setting the level width/height.
	this -> width = this -> tile_map -> getMapWidth();
	this -> height = this -> tile_map -> getMapHeight();
	SDL_Rect bounds = {0, 0, (int)this -> width, (int)this -> height};
	this -> quadTree = new QuadTree( 0, bounds );

	this -> background = Game::instance().getResources().get( "res/images/lv1_background.png" );

	// Loading checkpoint image.
	for ( int i = 0; i < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++i )
	{
		this -> checkpoints.push_back( Game::instance().getResources().get( "res/images/checkpoint.png" ));
	}

	this -> image = Game::instance().getResources().get( "res/images/potion.png" );

	// Getting information from lua script.
	LuaScript luaLevel1( "lua/Level1.lua" );
	const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get<std::string>(
		"level.player.spriteSheet" );
	const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get<std::string>(
		"level.audio.background" );
	const std::string PATH_ENEMY = luaLevel1.unlua_get<std::string>( "level.enemy" );

	/*Changing the music.
	Game::instance().get_audio_handler().change_music(PATH_BACKGROUND_AUDIO);

	Loading the player and the camera*/

	Player* level_player = nullptr;

	// Verifying number of games saved in the slots.
	if ( Game::instance().get_saves().is_saved( Game::instance().current_slot )
       && Game::instance().get_saves().get_saved_level( Game::instance().current_slot ) == MAX_NUMBER_OF_SLOTS )
	{
		double saved_x_position = 0.0;
		double saved_y_position = 0.0;

		Game::instance().get_saves().get_player_position( saved_x_position, saved_y_position, Game::instance().current_slot );

		level_player = new Player( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );

	} else
	{
		level_player = new Player( this -> tile_map -> get_initial_x(), this -> tile_map -> get_initial_y(),
		 PATH_PLAYER_SPRITE_SHEET );
	}

	Camera *level_camera = new Camera( level_player );

	this -> player_Hud = new PlayerHUD( level_player );

	// Load all the enemies from the tile_map.
	for ( unsigned int i = 0; i < this -> tile_map -> get_enemies_x().size(); i++ )
	{
		Enemy* enemy = new Enemy( this -> tile_map -> get_enemies_x().at(i),
			this -> tile_map -> get_enemies_y().at(i), PATH_ENEMY,
			this -> tile_map -> get_enemies_patrol().at(i), 0.0 );

		if ( Game::instance().get_saves().is_saved(Game::instance().current_slot)
    && Game::instance().get_saves().get_saved_level(Game::instance().current_slot) == 5 )
		{
			if ( Game::instance().get_saves().is_enemy_dead(i, Game::instance().current_slot ))
			{
				enemy -> set_dead(true);
			}
		}
		enemy -> setLevelWH( this -> width, this -> height );
	}

	// Loading documents.
	Document* document4 = new Document( 143*64, 35*64, "res/images/documentSprite.png", "res/images/Documents/d4.png" );
	this -> documents.push_back( document4 );

	Document* document5 = new Document( 143*64, 35*64, "res/images/documentSprite.png", "res/images/Documents/d5.png" );
	this -> documents.push_back( document5 );

	Document* document6 = new Document( 143*64, 35*64, "res/images/documentSprite.png", "res/images/Documents/d6.png" );
	this -> documents.push_back( document6 );

	// Setting the player and the camera.
	set_player( level_player );
	Enemy::points_life = this -> player -> life;

	set_camera( level_camera );

	Game::instance().get_fade().fade_out( 0, 0.002 );
}

/**
* Unloads everything that was loaded.
* @see LevelFive::load()
*/
void LevelFive::unload()
{
	Log(DEBUG) << "\tUnloading level 5...";

	clean_entities();
	clear_enemies();
	clear_documents();

	// Clearing caught items.
	for (int i = 0; i < NUMBER_ITEMS; ++i)
	{
		caught_items[i] = false;
	}

	//this -> checkpointVisited = false;
}

/**
* Updates the objects within the Level.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void LevelFive::update( const double DELTA_TIME ) 
{
	// Populating the QuadTree.
	this -> quadTree -> setObjects(this -> tile_map -> getCollisionRects());

	// Updating the entities, using the QuadTree.
	std::vector<CollisionRect> return_objects;
	for ( auto entity : this -> entities )
	{
		return_objects.clear();
		this -> quadTree -> retrieve( return_objects, entity -> get_bounding_box() );
		entity -> setCollisionRects( return_objects );
		entity -> update( DELTA_TIME );
	}

	// Updating the enemies.
	for ( auto enemy : this -> enemies )
	{
		return_objects.clear();
		this -> quadTree -> retrieve( return_objects, enemy -> get_bounding_box() );
		enemy -> setCollisionRects( return_objects );
		enemy -> update( DELTA_TIME );
	}

	// Set to GameOver if the player is dead.
	if ( this -> player -> isDead() )
	{
		Game::instance().setState( Game::GStates::GAMEOVER );
		return;
	}

	// Updating the potions.
	for ( auto potion : this -> player -> potions )
	{
		return_objects.clear();
		this -> quadTree -> retrieve( return_objects, potion -> get_bounding_box() );
		potion -> setCollisionRects( return_objects );
	}

	/*@todo Maybe refactor this static Enemy::px, Enemy::py.
	Updating player info for the enemies.*/
	Enemy::px = this -> player -> x;
	Enemy::py = this -> player -> y;
	Enemy::position_vulnerable = this -> player -> is_vulnerable;

	// Updating number os potions left.
	for ( int i = 0; i < NUMBER_ITEMS; ++i )
	{
		if ( Collision::rects_collided(this -> player -> get_bounding_box(),
    {items[0][i], items[1][i], 192, 192}) && caught_items[i] == false )
		{
			this -> player -> addPotions(3);
			caught_items[i]=true;
		}
	}

	// Updating player's life.
	if ( this -> player -> life != Enemy::points_life )
	{
		if ( this -> player -> is_vulnerable )
		{
			this -> player -> life--;
			Enemy::points_life = this -> player -> life;
			this -> player -> changeState( Player::player_states::HITED );
			this -> player -> is_vulnerable = false;
		} else
		{

		}
	}

	// Updating the HUD.
	this -> player_Hud -> update();

	// Updating the camera.
	this -> camera -> update();

	// Set next level if end is reached.
	if ( this -> player -> reached_level_end )
	{
		Game::instance().transitionTo = Game::GStates::LEVEL_BOSS;
		Game::instance().setState( Game::GStates::TRANSITION );
		return;
	}

	// Updating the potion/enemy collision.
	for ( auto potion : this -> player -> potions )
	{
		for( auto enemy : this -> enemies )
		{
			if ( Collision::rects_collided(potion -> get_bounding_box(), enemy -> get_bounding_box()))
			{
				if ( potion -> activated )
				{
				  if( enemy -> life > 0 && this -> player -> can_attack )
					{
						enemy -> life -= 100;
					  potion -> activated = false;
					}
					// Log(DEBUG) << "Enemy Life = " << enemy -> life;

					if ( enemy -> life <= 0 )
					{
						enemy -> changeState(Enemy::EStates::DEAD);
					}
				}
			}
		}
	}

	// Updating the player attack/enemy collision.
	for ( auto enemy : this -> enemies )
	{
		if ( Collision::rects_collided( this -> player -> get_bounding_box(), enemy -> get_bounding_box()) ){
			if ( this -> player -> is_right != enemy -> is_right )
				if ( this -> player -> is_current_state(Player::player_states::ATTACK ) ||
        this -> player -> is_current_state( Player::player_states::ATTACKMOVING) )
        {
					if ( enemy -> life > 0 && this -> player -> can_attack )
					{
						enemy -> life -= this -> player -> attack_strength;
						this -> player -> can_attack = false;
					}
					// Log(DEBUG) << "Enemy Life = " << enemy -> life;

					if ( enemy -> life <= 0 )
						enemy -> changeState( Enemy::EStates::DEAD );
				}
		}
	}

	// Saving the game state.
	for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
	{
		if ( !this -> checkpoints_visited[j] && this -> player -> get_bounding_box().x >= checkpoints_X[j]
				&& this -> player -> get_bounding_box().x <= checkpoints_X[j] + 100
        && this -> player -> get_bounding_box().y >= checkpoints_Y[j]
				&& this -> player -> get_bounding_box().y <= checkpoints_Y[j] + 200 )
		{
			this -> checkpoints[j] = Game::instance().getResources().get("res/images/checkpoint_visited.png");
			Game::instance().get_saves().saveLevel(5, this -> player, this -> enemies, Game::instance().current_slot);
			this -> checkpoints_visited[j] = true;
		}
	}

	// Documents check.
	for ( auto document : this -> documents )
	{
		if ( Collision::rects_collided(this -> player -> get_bounding_box(), document -> get_bounding_box()) ){
			document -> should_render = true;
		} else
		{
			document -> should_render = false;
		}
	}
}

/**
* Renders the level.
* Always renders on 0,0 position.
* @see Sprite::render()
 */
void LevelFive::render()
{
	// Getting camera.
	const int CAMERA_X = this -> camera -> getClip().x;
	const int CAMERA_Y = this -> camera -> getClip().y;

	this -> background -> render( 0, 0 );

	// Rendering checkpoint's camera.
	for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
	{
		this -> checkpoints[j] -> render( this -> checkpoints_X[j] - CAMERA_X, this -> checkpoints_Y[j] - CAMERA_Y );
	}

	// Rendering the tiles in the TileMap.
	this -> tile_map -> render( CAMERA_X, CAMERA_Y );

	this -> player_Hud -> render();

	// Rendering all the enemies in the list.
	for ( auto enemy : this -> enemies )
	{
		enemy -> render( CAMERA_X, CAMERA_Y );
	}

	// Rendering all the entities in the list.
	for ( auto entity : this -> entities )
	{
        entity -> render( CAMERA_X, CAMERA_Y );
	}

	// Rendering images.
	for ( unsigned int i = 0; i < NUMBER_ITEMS; i++ )
	{
		if ( this -> image != nullptr && caught_items[i] == false )
		{
			this -> image -> Sprite::render( (items[0][i]+60) - CAMERA_X, ( (items[1][i]) - CAMERA_Y) );
		}
	}

	// Document text image.
	for ( auto document : this -> documents )
	{
		document -> render( CAMERA_X, CAMERA_Y );
		if ( document -> should_render )
		{
			document -> renderDocumentText();
		}
	}
}
