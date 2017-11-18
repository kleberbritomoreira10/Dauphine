/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @LevelTwo.cpp
* The first level of the game derived from Level class.
*/

#include "LevelTwo.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"
#include <assert.h>
#include <cstddef>

#define CHECKPOINTS_X_AXIS_1 4090
#define NUMBER_POTION 3
#define TOTAL_POTIONS 192          // 3 * 64
#define CHECKPOINTS_Y_AXIS_1 7870 
#define NUMBER_CHECKPOINT 2
#define CHECKPOINTS_X_AXIS_2 2776
#define CHECKPOINTS_Y_AXIS_2 1700
#define PERCENTAGE 0 
#define TIME 0.002

/**
* The constructor.
* @see Level::Level()
*/
LevelTwo::LevelTwo () :
  Level (),
  items{ { CHECKPOINTS_X_AXIS_1 - TOTAL_POTIONS, CHECKPOINTS_Y_AXIS_1 - TOTAL_POTIONS, 0, 0 },{ CHECKPOINTS_X_AXIS_2, CHECKPOINTS_Y_AXIS_2, 0, 0 } },
  caught_items{ false, false, true, false }
{
  this -> changeCheckpoints ( NUMBER_CHECKPOINT, { CHECKPOINTS_X_AXIS_1, CHECKPOINTS_Y_AXIS_1 }, { CHECKPOINTS_X_AXIS_2, CHECKPOINTS_Y_AXIS_2 } );
}

/**
* The destructor.
*/
LevelTwo::~LevelTwo ()
{

}

/**
* Loads the level.
* From the Level1.lua script, loads all the necessary objects.
*/
void LevelTwo::load ()
{
  // Changing the music.
  Game::instance (). get_audio_handler ().change_music ( "res/audio/lv1.wav" );

  // Loading the tile/tilemap.
  this -> tile_map = new TileMap( "res/maps/level2.tmx" );

  // Setting the level width/height.
  this -> width = this -> tile_map -> getMapWidth ();
  this -> height = this -> tile_map -> getMapHeight ();
  SDL_Rect bounds = {0, 0, ( int )this -> width, ( int )this -> height};
  this -> quadTree = new QuadTree ( 0, bounds );

  this -> background = Game::instance (). getResources ().get ( "res/images/lv1_background.png" );
  for ( int i = 0; i < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++i )
  {
    this -> checkpoints.push_back ( Game::instance ().getResources ().get ( "res/images/checkpoint.png" ) );
  }
  this -> image = Game::instance ().getResources ().get ( "res/images/potion.png" );

  // Changing the music.
  // Game::instance().get_audio_handler().change_music(PATH_BACKGROUND_AUDIO);

  Player *level_player = nullptr;  // Loading the player.

  level_player = create_player();

  assert( level_player != nullptr );
  this -> player_Hud = new PlayerHUD ( level_player );

  load_enemy();

  // Finally, setting the player and the camera.
  set_player ( level_player );
  Enemy::points_life = this -> player -> life;

  Camera *level_camera = new Camera ( level_player ); // Loading the camera.
  set_camera ( level_camera );
  assert( level_camera != nullptr );

  Game::instance().get_fade().fade_out ( PERCENTAGE, TIME );
}

Player *LevelTwo::create_player()
{
  Player *level_player = nullptr;  // Loading the player.

  LuaScript luaLevel1 ( "lua/Level1.lua" );
  const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get<std::string> (
    "level.player.spriteSheet" );
  const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get<std::string> (
    "level.audio.background" );

  if ( Game::instance ().get_saves ().is_saved ( Game::instance ().current_slot ) 
      && Game::instance ().get_saves ().get_saved_level ( Game::instance ().current_slot ) == 2 )
  {
    double saved_x_position = 0.0;  //Position of the player that was saved on the x axis  
    double saved_y_position = 0.0;  //Position of the player that was saved on the y axis

    Game::instance ().get_saves ().get_player_position ( saved_x_position, saved_y_position, Game::instance ().current_slot );

    level_player = new Player ( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
  } else
    {
      level_player = new Player ( this -> tile_map -> get_initial_x (), this -> tile_map -> get_initial_y (), PATH_PLAYER_SPRITE_SHEET );
    }

  return level_player;    
}

void LevelTwo::load_enemy()
{ 
  // Getting information from lua script.
  LuaScript luaLevel1 ( "lua/Level1.lua" );
  
  const std::string PATH_ENEMY = luaLevel1.unlua_get<std::string> ( "level.enemy" );
  // Load all the enemies from the tile_map.
  
  for ( unsigned int i = 0; i < this -> tile_map->get_enemies_x().size(); i++ )
  {
    Enemy *enemy = new Enemy ( this -> tile_map -> get_enemies_x().at( i ) ,
      this -> tile_map -> get_enemies_y ().at ( i ), PATH_ENEMY,
      this -> tile_map -> get_enemies_patrol ().at( i ), 0.0 );

    if ( Game::instance().get_saves().is_saved ( Game::instance().current_slot ) )
    {
      if ( Game::instance().get_saves().is_enemy_dead ( i, Game::instance().current_slot ) &&
          Game::instance().get_saves().get_saved_level ( Game::instance().current_slot ) == 2 )
      {
        enemy -> set_dead ( true );
        assert( enemy != nullptr);
      } else
        {
          //No action   
        }
    } else
      {
        //No action 
      }
    enemy -> setLevelWH ( this -> width, this -> height );
    assert( width > 0 || height > 0);
    this -> enemies.push_back ( enemy );
  }
}

/**
* Updates the objects within the Level.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void LevelTwo::unload ()
{
  clean_entities ();
  clear_enemies ();
  clear_documents ();

  for ( int i = 0; i < NUMBER_ITEMS; ++i )
  {
    caught_items [ i ] = false;
  }

  //this->checkpointVisited = false;  
}

/**
* Unloads everything that was loaded.
* @see LevelTwo::load()
*/
void LevelTwo::update ( const double DELTA_TIME )
{ 
  assert( DELTA_TIME >= 0);
  // Populating the QuadTree.
  this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

  std::vector<CollisionRect> return_objects;

  return_objects = update_entity (return_objects, DELTA_TIME);

  // Updating the enemies.
  for ( auto enemy : this -> enemies )
  {
    return_objects.clear();
    this -> quadTree->retrieve ( return_objects, enemy->get_bounding_box () );
    enemy -> setCollisionRects ( return_objects );
    enemy -> update ( DELTA_TIME );
  }

  // Set to GameOver if the player is dead.
  if ( this -> player -> isDead() )
  {
    Game::instance().setState ( Game::GStates::GAMEOVER );
    return;
  } else
    {
      //No action
    }

  // Updating the potions.
  for ( auto potion : this -> player->potions )
  {
    return_objects.clear ();
    this -> quadTree -> retrieve ( return_objects, potion -> get_bounding_box() );
    potion -> setCollisionRects ( return_objects );
  }

  // @todo Maybe refactor this static Enemy::px, Enemy::py.
  // Updating player info for the enemies.
  Enemy::px = this -> player -> x;
  Enemy::py = this -> player -> y;
  Enemy::position_vulnerable = this -> player -> is_vulnerable;

  update_number_potion();

  if ( this -> player -> life != Enemy::points_life )
  {
    if ( this -> player -> is_vulnerable )
    {
      this -> player -> life--;
      Enemy::points_life = this -> player -> life;
      this -> player -> changeState ( Player::player_states::HITED );
      this -> player -> is_vulnerable = false;
    } else
      {
        //No action 
      }
  }

  // Updating the HUD.
  this -> player_Hud -> update ();

  // Updating the camera.
  this -> camera -> update ();

  // Set next level if end is reached.
  if ( this -> player -> reached_level_end )
  {
    Game::instance ().transitionTo = Game::GStates::LEVEL_THREE;
    Game::instance ().setState ( Game::GStates::TRANSITION );
    return;
  } else
    {
      //No action
    }

  update_potion();   
  
  update_collision();

  save();

  document_check();  

}

void LevelTwo::update_number_potion()
{
//update number of potions where limit of number itens = 4 for level two. 
  for ( int i = 0; i < NUMBER_ITEMS; ++i )
  { 
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), 
      {items [ 0 ] [ i ], items [ 1 ] [ i ], TOTAL_POTIONS, TOTAL_POTIONS}) && caught_items [ i ] == false )
    {
      this -> player -> addPotions(NUMBER_POTION);
      caught_items [ i ] =true;
    } else
      {
        //No action
      }
  }
}

void LevelTwo::update_potion()
{
// Updating the potion/enemy collision.
  for ( auto potion : this -> player -> potions )
  {
    for ( auto enemy : this -> enemies )
    {
      if ( Collision::rects_collided ( potion -> get_bounding_box (), enemy -> get_bounding_box () ) )
      {
        if ( potion -> activated )
        {     
          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= 100;
            potion -> activated = false;
          } else
            {
              // No action
            }

          if ( enemy -> life <= 0 )
          {
            enemy -> changeState ( Enemy::EStates::DEAD );
          } else
            {
              // No action
            }
        } else
          {
            // No action
          }
      } else
        {
          // No action
        }
    }
  }
}

void LevelTwo::update_collision()
{
  // Updating the player attack/enemy collision.
  for ( auto enemy : this -> enemies )
  {
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), enemy -> get_bounding_box () ) )
    {
      if ( this -> player -> is_right != enemy -> is_right )
      {
        if ( this -> player -> is_current_state( Player::player_states::ATTACK ) || 
            this -> player -> is_current_state( Player::player_states::ATTACKMOVING ) )
        {
          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= this -> player -> attack_strength;
            this -> player -> can_attack = false;
          } else
            {
              // No action
            }
         
          if ( enemy -> life <= 0 )
          {
            enemy -> changeState ( Enemy::EStates::DEAD );
          } else
            {
              // No action
            }
        } else
          {
            // No action
          }
        } else {
            //No action
          }
    } else
      {
        // No action
      }
  }

}

void LevelTwo::document_check()
{
  // Documents check
  for ( auto document : this -> documents )
  {
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), document -> get_bounding_box () ) )
    {
      document -> should_render = true;
    } else
      {
        document -> should_render = false;
      }
  }
}

void LevelTwo::save()
{
//Saving the game state
  for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
  {
    if ( !this -> checkpoints_visited [ j ] && this -> player -> get_bounding_box().x >= checkpoints_X [ j ] 
        && this -> player -> get_bounding_box().x <= checkpoints_X [ j ] + 100 &&
            this -> player -> get_bounding_box().y >= checkpoints_Y [ j ]
        && this -> player -> get_bounding_box().y <= checkpoints_Y [ j ] + 200 )
    {
      this -> checkpoints [ j ] = Game::instance ().getResources ().get( "res/images/checkpoint_visited.png" );
      Game::instance ().get_saves ().saveLevel ( 2, this -> player, this -> enemies, Game::instance ().current_slot );
      this -> checkpoints_visited [ j ] = true;
    } else
      {
        // No action
      }
  }
}

std::vector<CollisionRect> LevelTwo::update_entity (std::vector<CollisionRect> return_objects, const double DELTA_TIME)
{ 
  // Updating the entities, using the QuadTree.
  
  for ( auto entity : this -> entities )
  {
    return_objects.clear ();
    this->quadTree -> retrieve ( return_objects, entity -> get_bounding_box () );
    entity -> setCollisionRects ( return_objects );
    entity -> update( DELTA_TIME );
  }

  return return_objects;
}

/**
* Renders the level.
* Always renders on 0,0 position.
* @see Sprite::render()
*/
void LevelTwo::render ()
{
  const int CAMERA_X = this -> camera -> getClip ().x;  //Declaring the x position of the camera at level 2.
  const int CAMERA_Y = this -> camera -> getClip ().y;  //Declaring the y position of the camera at level 2.

  this -> background -> render ( 0, 0 );

  for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
  {
    this -> checkpoints [ j ] -> render ( this -> checkpoints_X [ j ] - CAMERA_X, this -> checkpoints_Y [ j ] - CAMERA_Y );
  }

  // Render the tiles in the TileMap.
  this -> tile_map -> render( CAMERA_X, CAMERA_Y );

  this -> player_Hud -> render ();

  for ( auto enemy : this -> enemies )
  {
    enemy -> render ( CAMERA_X, CAMERA_Y );
  }

  // Render all the entities in the list.
  for ( auto entity : this -> entities )
  {
    entity -> render( CAMERA_X, CAMERA_Y );
  }

  for ( unsigned int i = 0; i < NUMBER_ITEMS; i++ )
  {
    if ( this -> image != nullptr && caught_items [ i ] == false )
    {
      
      this -> image -> Sprite::render ( ( items [ 0 ] [ i ] + 60 ) - CAMERA_X, ( ( items [ 1 ] [ i ] ) - CAMERA_Y ) );
    } else
      {
        // No action
      }
  }

  // Document text image
  for ( auto document : this -> documents )
  {
    document -> render( CAMERA_X, CAMERA_Y );
    if ( document -> should_render )
    {
      document -> renderDocumentText ();
    } else
      {
       // No action
      }
  }
}
