/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LevelFour.cpp
 * File responsible for implementing the phase 4 aspects of the game.
 * Factors such as numbering of items that can be captured, HUD, enemies and aspects of basic settings.
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "LevelFour.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"
#include "Document.h"

/*
 * Numbering for items that can be captured in phase 4.
 */
LevelFour::LevelFour() :
  Level(),
  items{ { 207, 11261, 6800, 10000 },{ 5600, 2050, 5850, 2712 } },
  caught_items{ false, false, false, true }
{
  this -> changeCheckpoints( 2, { 58*64,71*64 }, { 113*64,20*64 } );
}

LevelFour::~LevelFour()
{

}

/*
 * Initializing aspects related to phase 4 of the game.
 */
void LevelFour::load()
{
  // Changing the music.
  Game::instance().get_audio_handler().change_music("res/audio/lv4.wav");

  // Loading the tile/tilemap.

  assert( this -> tile_map != nullptr );        
  this -> tile_map = new TileMap("res/maps/level4.tmx");

  // Setting the level width/height.
  this -> width = this -> tile_map -> getMapWidth();
  this -> height = this -> tile_map -> getMapHeight();
  SDL_Rect bounds = { 0, 0, ( int ) this -> width, ( int ) this -> height };
  assert( this -> quadTree != nullptr );        
  this -> quadTree = new QuadTree( 0, bounds );

  //Setting the background image
  this -> background = Game::instance().getResources().get("res/images/lv1_background.png");
  for ( int i = 0; i < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++i )
  {
    this -> checkpoints.push_back( Game::instance().getResources().get("res/images/checkpoint.png") );
  }
  //Referencing the image of the potion
  this -> image = Game::instance().getResources().get("res/images/potion.png");

  /*
   * Getting information from lua script.
   * Changing the music.
   * Game::instance().get_audio_handler().change_music(PATH_BACKGROUND_AUDIO);
   */
  LuaScript luaLevel1("lua/Level1.lua");
  const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get<std::string>("level.player.spriteSheet");
  const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get<std::string>("level.audio.background");
  const std::string PATH_ENEMY = luaLevel1.unlua_get<std::string>("level.enemy");

  // Loading the player and the camera.
  Player* level_player = nullptr;

  //check to see if the player is in stage 4, and if so, determine the position on the x-axis and y
  if ( Game::instance().get_saves().is_saved(Game::instance().current_slot) && Game::instance().get_saves().get_saved_level(Game::instance().current_slot) == 4 )
  {
    double saved_x_position = 0.0;
    double saved_y_position = 0.0;

    Game::instance().get_saves().get_player_position(saved_x_position, saved_y_position, Game::instance().current_slot);

    assert( level_player != nullptr );          
    level_player = new Player( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
  } else {
      assert( level_player != nullptr );          
      level_player = new Player( this -> tile_map -> get_initial_x(), this -> tile_map -> get_initial_y(), PATH_PLAYER_SPRITE_SHEET);
    }

  Camera* level_camera = new Camera( level_player );

  //Instantiating elements of the game.
  assert( this -> player_Hud != nullptr );            
  this -> player_Hud = new PlayerHUD( level_player );

  // Load all the enemies from the tile_map.
  for ( unsigned  int i = 0; i < this -> tile_map -> get_enemies_x().size(); i++ )
  {
    Enemy* enemy = new Enemy( this -> tile_map -> get_enemies_x().at(i),
    this -> tile_map -> get_enemies_y().at(i), PATH_ENEMY,
    this -> tile_map -> get_enemies_patrol().at(i), 0.0);

    if ( Game::instance().get_saves().is_saved(Game::instance().current_slot) )
    {
      if ( Game::instance().get_saves().is_enemy_dead(i, Game::instance().current_slot) && Game::instance().get_saves().get_saved_level(Game::instance().current_slot) == 4 )
      {
        enemy->set_dead(true);
      } else {
        enemy->set_dead(false);
      }
    } else {
      enemy->set_dead(false);      
    }
    enemy -> setLevelWH( this -> width, this -> height );
    this -> enemies.push_back( enemy );
  }

  // Documents;
  Document* document1 = new Document( 28*64, 64*64, "res/images/documentSprite.png", "res/images/Documents/d1.png");
  this -> documents.push_back( document1 );

  Document* document2 = new Document(75*64, 34*64, "res/images/documentSprite.png", "res/images/Documents/d2.png");
  this -> documents.push_back( document2 );

  Document* document3 = new Document(151*64, 25*64, "res/images/documentSprite.png", "res/images/Documents/d3.png");
  this -> documents.push_back( document3 );

  // Setting the player and the camera.
  set_player( level_player );
  Enemy::points_life = this -> player -> life;

  set_camera( level_camera );

  Game::instance().get_fade().fade_out( 0, 0.002 );
}

/*
 * Clearing phase 4.
 */
void LevelFour::unload()
{
  Log(DEBUG) << "\tUnloading level 4...";

  clean_entities();
  clear_enemies();
  clear_documents();

  for ( int i = 0; i < NUMBER_ITEMS; ++i )
  {
    caught_items[i] = false;
  }
  //this->checkpointVisited = false;
}

/*
 * Updating all aspects of phase 4.
 * @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
 */
void LevelFour::update( const double DELTA_TIME )
{
  assert( DELTA_TIME >= 0 );
  // Populating the QuadTree.
  this -> quadTree -> setObjects( this -> tile_map -> getCollisionRects() );

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
    this -> quadTree -> retrieve( return_objects, enemy->get_bounding_box() );
    enemy -> setCollisionRects( return_objects );
    enemy -> update( DELTA_TIME );
  }

  // Set to GameOver if the player is dead.
  if ( this -> player -> isDead() )
  {
    Game::instance().setState( Game::GStates::GAMEOVER );
    return;
  } else {
    // No Action.
  }

  // Updating the potions.
  for ( auto potion : this -> player -> potions )
  {
    return_objects.clear();
    this -> quadTree -> retrieve( return_objects, potion->get_bounding_box() );
    potion -> setCollisionRects( return_objects );
  }

  // @todo Maybe refactor this static Enemy::px, Enemy::py.
  // Updating player info for the enemies.
  Enemy::px = this -> player -> x;
  Enemy::py = this -> player -> y;
  Enemy::position_vulnerable = this -> player -> is_vulnerable;

  //If collect item, update the amount of potions to 3
  for ( int i = 0; i < NUMBER_ITEMS; ++i )
  {
    if ( Collision::rects_collided( this -> player->get_bounding_box(), {items[0][i], items[1][i], 192, 192}) &&
      caught_items[i] == false )
    {
      this -> player -> addPotions(3);
      caught_items[i] = true;
    } else {
      // No Action..
    }
  }

  //Verify if life player is vulnerable
  if ( this -> player -> life != Enemy::points_life )
  {
    if ( this -> player -> is_vulnerable )
    {
      this -> player -> life--;
      Enemy::points_life = this -> player -> life;
      this -> player -> changeState( Player::player_states::HITED );
      this -> player -> is_vulnerable = false;
    } else {
        // No Actiion.
    }
  } else {
    // No Action.
  }

  // Updating the HUD.
  this -> player_Hud -> update();

  // Updating the camera.
  this -> camera -> update();

  // Set next level if end is reached.
  if ( this -> player -> reached_level_end )
  {
    Game::instance().transitionTo = Game::GStates::LEVEL_FIVE;
    Game::instance().setState(Game::GStates::TRANSITION);
    return;
  } else {
    // No Action.
  }

  // Updating the potion/enemy collision.
  for ( auto potion : this -> player -> potions )
  {
    for ( auto enemy : this->enemies )
    {
      if ( Collision::rects_collided( potion -> get_bounding_box(), enemy -> get_bounding_box()) )
      {
        if ( potion -> activated )
        {
          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= 100;
            potion -> activated = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;
          else if ( enemy -> life <= 0 )
          {
            enemy->changeState( Enemy::EStates::DEAD );
          } else {
            // No Action.
          }
        } else {
          // No Action.
        }
      } else {
        // No Action.
      }
    }
  }

  // Updating the player attack/enemy collision.
  for ( auto enemy : this -> enemies )
  {
    if ( Collision::rects_collided( this -> player -> get_bounding_box(), enemy -> get_bounding_box()) )
    {
      if ( this -> player -> is_right != enemy -> is_right ) { 
        if ( this -> player -> is_current_state( Player::player_states::ATTACK) || this->player->is_current_state(Player::player_states::ATTACKMOVING) )
        {

          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= this -> player -> attack_strength;
            this -> player -> can_attack = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;
          else if ( enemy -> life <= 0 )
          {
            enemy -> changeState( Enemy::EStates::DEAD );
          } else {
            // No Action.
          }
        } else {
          // No Action.
        }
      }
    } else {
      // No Action.
    }
  }

  //Saving the game state
  for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
  {
    if ( !this -> checkpoints_visited[j] && this -> player -> get_bounding_box().x >= checkpoints_X[j]
        && this -> player -> get_bounding_box().x <= checkpoints_X[j] + 100 && this -> player -> get_bounding_box().y >= checkpoints_Y[j] && this -> player -> get_bounding_box().y <= checkpoints_Y[j] + 200 )
    {
      this -> checkpoints[j] = Game::instance().getResources().get("res/images/checkpoint_visited.png");
      Game::instance().get_saves().saveLevel(4, this -> player, this -> enemies, Game::instance().current_slot );
      this -> checkpoints_visited[j] = true;
    } else {
      // No Action.
    }
  }

  // Documents check
  for ( auto document : this -> documents )
  {
    if ( Collision::rects_collided(this -> player -> get_bounding_box(), document -> get_bounding_box()) )
    {
      document->should_render = true;
    } else {
        document->should_render = false;
    }
  }
}

/*
 * Rendering all aspects of phase 4.
 */
void LevelFour::render()
{
  const int CAMERA_X = this -> camera -> getClip().x;
  const int CAMERA_Y = this -> camera -> getClip().y;

  this -> background->render(0, 0);

  for ( int j = 0; j < this->TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
  {
    this -> checkpoints[j] -> render( this -> checkpoints_X[j] - CAMERA_X, this -> checkpoints_Y[j] - CAMERA_Y );
  }

  // Render the tiles in the TileMap.
  this -> tile_map -> render( CAMERA_X, CAMERA_Y );

  this -> player_Hud -> render();

  for ( auto enemy : this -> enemies )
  {
    enemy -> render( CAMERA_X, CAMERA_Y );
  }

  // Render all the entities in the list.
  for ( auto entity : this -> entities )
  {
    entity -> render( CAMERA_X, CAMERA_Y );
  }

  // Potion refill
  for ( unsigned int i = 0; i < NUMBER_ITEMS; i++ )
  {
    if ( this -> image != nullptr && caught_items[i] == false )
    {
      this -> image -> Sprite::render( ( items[0][i]+60) - CAMERA_X, ((items[1][i]) - CAMERA_Y) );
    } else {
      // No Action.
    }
  }

  // Document text image
  for ( auto document : this -> documents )
  {
    document -> render( CAMERA_X, CAMERA_Y );
    if ( document -> should_render )
    {
      document -> renderDocumentText();
    } else {
      // No action.
    }
  }
}
