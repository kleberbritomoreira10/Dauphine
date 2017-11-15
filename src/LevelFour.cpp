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

#define CHECKPOINTS_X_AXIS_1 3712  // 58 * 64
#define CHECKPOINTS_Y_AXIS_1 4544  // 71 * 64
#define CHECKPOINTS_X_AXIS_2 7232  // 113 * 64
#define CHECKPOINTS_Y_AXIS_2 1280  // 20 * 64
#define NUMBER_CHECKPOINT 2
#define NUMBER_LEVEL 4
#define POSITION_X_ITEM_1 207
#define POSITION_X_ITEM_2 11261
#define POSITION_X_ITEM_3 6800
#define POSITION_X_ITEM_4 10000
#define POSITION_Y_ITEM_1 5600
#define POSITION_Y_ITEM_2 2050
#define POSITION_Y_ITEM_3 2850
#define POSITION_Y_ITEM_4 2712
#define TOTAL_LIFE 100
#define TOTAL_POTIONS 192
#define ZERO 0

/*
 * Numbering for items that can be captured in phase 4.
 */
LevelFour::LevelFour() :
  Level(),
  items{ 
    { POSITION_X_ITEM_1, POSITION_X_ITEM_2, POSITION_X_ITEM_3, POSITION_X_ITEM_4 },
    { POSITION_Y_ITEM_1, POSITION_Y_ITEM_2, POSITION_Y_ITEM_3, POSITION_Y_ITEM_4 } 
  },
  caught_items{ false, false, false, true }
{
  this -> changeCheckpoints( NUMBER_CHECKPOINT, 
    { CHECKPOINTS_X_AXIS_1, CHECKPOINTS_Y_AXIS_1 }, 
    { CHECKPOINTS_X_AXIS_2, CHECKPOINTS_Y_AXIS_2 }
  );
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

  this -> tile_map = new TileMap("res/maps/level4.tmx");
  assert( this -> tile_map != nullptr );

  // Setting the level width/height.
  this -> width = this -> tile_map -> getMapWidth();
  this -> height = this -> tile_map -> getMapHeight();
  SDL_Rect bounds = { ZERO, ZERO, ( int ) this -> width, ( int ) this -> height };
  this -> quadTree = new QuadTree( ZERO, bounds );
  assert( this -> quadTree != nullptr );

  //Setting the background image
  this -> background = Game::instance().getResources().get("res/images/lv1_background.png");
  for ( int i = ZERO; i < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++i )
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
  if ( Game::instance().get_saves().is_saved(Game::instance().current_slot) && Game::instance().get_saves().get_saved_level(Game::instance().current_slot) == NUMBER_LEVEL )
  {
    double saved_x_position = 0.0;
    double saved_y_position = 0.0;

    Game::instance().get_saves().get_player_position(saved_x_position, saved_y_position, Game::instance().current_slot);

    level_player = new Player( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
  } else {
      level_player = new Player( this -> tile_map -> get_initial_x(), this -> tile_map -> get_initial_y(), PATH_PLAYER_SPRITE_SHEET);
    }

  Camera* level_camera = new Camera( level_player );

  //Instantiating elements of the game.
  this -> player_Hud = new PlayerHUD( level_player );
  assert( this -> player_Hud != nullptr );

  // Load all the enemies from the tile_map.
  for ( unsigned  int i = ZERO; i < this -> tile_map -> get_enemies_x().size(); i++ )
  {
    Enemy* enemy = new Enemy( this -> tile_map -> get_enemies_x().at(i),
    this -> tile_map -> get_enemies_y().at(i), PATH_ENEMY,
    this -> tile_map -> get_enemies_patrol().at(i), 0.0);

    if ( Game::instance().get_saves().is_saved(Game::instance().current_slot) )
    {
      if ( Game::instance().get_saves().is_enemy_dead(i, Game::instance().current_slot) && Game::instance().get_saves().get_saved_level(Game::instance().current_slot) == NUMBER_LEVEL )
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

  Game::instance().get_fade().fade_out( ZERO, 0.002 );
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

  for ( int i = ZERO; i < NUMBER_ITEMS; ++i )
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
  assert( DELTA_TIME >= ZERO );
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
  for ( int i = ZERO; i < NUMBER_ITEMS; ++i )
  {
    if ( Collision::rects_collided( this -> player->get_bounding_box(), {items[0][i], items[1][i], TOTAL_POTIONS, TOTAL_POTIONS}) &&
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
          if ( enemy -> life > ZERO && this -> player -> can_attack )
          {
            enemy -> life -= TOTAL_LIFE;
            potion -> activated = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;
          else if ( enemy -> life <= ZERO )
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

          if ( enemy -> life > ZERO && this -> player -> can_attack )
          {
            enemy -> life -= this -> player -> attack_strength;
            this -> player -> can_attack = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;
          else if ( enemy -> life <= ZERO )
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
  for ( int j = ZERO; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
  {
    if ( !this -> checkpoints_visited[j] && this -> player -> get_bounding_box().x >= checkpoints_X[j]
        && this -> player -> get_bounding_box().x <= checkpoints_X[j] + TOTAL_LIFE && this -> player -> get_bounding_box().y >= checkpoints_Y[j] && this -> player -> get_bounding_box().y <= checkpoints_Y[j] + 200 )
    {
      this -> checkpoints[j] = Game::instance().getResources().get("res/images/checkpoint_visited.png");
      Game::instance().get_saves().saveLevel(NUMBER_LEVEL, this -> player, this -> enemies, Game::instance().current_slot );
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

  this -> background->render(ZERO, ZERO);

  for ( int j = ZERO; j < this->TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
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
  for ( unsigned int i = ZERO; i < NUMBER_ITEMS; i++ )
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
