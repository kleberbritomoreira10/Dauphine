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

/**
* The constructor.
* @see Level::Level()
*/
LevelTwo::LevelTwo () :
  Level (),
  items{ { 4090 - 3 * 64, 7870 - 3 * 64, 0, 0 },{ 2776, 1700, 0, 0 } },
  caught_items{ false, false, true, false }
{
  this -> changeCheckpoints ( 2, { 4090, 7870 }, { 2776, 1700 } );
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
  for( int i = 0; i < this -> NUMBER_OF_CHECKPOINTS; ++i )
  {
    this -> checkpoints.push_back ( Game::instance ().getResources ().get ( "res/images/checkpoint.png" ) );
  }
  this -> image = Game::instance ().getResources ().get ( "res/images/potion.png" );

  // Getting information from lua script.
  LuaScript luaLevel1 ( "lua/Level1.lua" );
  const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get<std::string> (
    "level.player.spriteSheet" );
  // Alert!: in variable PATH_BACKGROUND_AUDIO is assigned a value is never used.
  const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get<std::string> (
    "level.audio.background" );
  const std::string PATH_ENEMY = luaLevel1.unlua_get<std::string> ( "level.enemy" );

  // Changing the music.
  // Game::instance().get_audio_handler().change_music(PATH_BACKGROUND_AUDIO);

  Player *level_player = nullptr;  // Loading the player.
  
  if ( Game::instance ().get_saves ().is_saved ( Game::instance ().current_slot ) 
      && Game::instance ().get_saves ().get_saved_level ( Game::instance ().current_slot ) == 2 )
  {
    double saved_x_position = 0.0;  //Position of the player that was saved on the x axis  
    double saved_y_position = 0.0;  //Position of the player that was saved on the y axis

    Game::instance ().get_saves ().get_player_position ( saved_x_position, saved_y_position, Game::instance ().current_slot );

    level_player = new Player ( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
  }
  else
  {
    level_player = new Player ( this -> tile_map -> get_initial_x (), 
                          this -> tile_map -> get_initial_y (), PATH_PLAYER_SPRITE_SHEET );
  }
  
  Camera *level_camera = new Camera ( level_player ); // Loading the camera.
  
  this -> player_Hud = new PlayerHUD ( level_player );

  // Load all the enemies from the tile_map.
  for( unsigned int i = 0; i < this -> tile_map->get_enemies_x().size(); i++ )
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
      }
    }
    enemy -> setLevelWH ( this -> width, this -> height );
    this -> enemies.push_back ( enemy );
  }

  // Finally, setting the player and the camera.
  set_player ( level_player );
  Enemy::points_life = this -> player -> life;

  set_camera ( level_camera );

  Game::instance ().get_fade ().fade_out ( 0, 0.002 );
}

/**
* Updates the objects within the Level.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void LevelTwo::unload ()
{
  Log ( DEBUG ) << "\tUnloading level 2...";

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
  // Populating the QuadTree.
  this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

  // Updating the entities, using the QuadTree.
  std::vector<CollisionRect> return_objects;
  for( auto entity : this -> entities )
  {
    return_objects.clear ();
    this->quadTree -> retrieve ( return_objects, entity -> get_bounding_box () );
    entity -> setCollisionRects ( return_objects );
    entity -> update( DELTA_TIME );
  }

  // Updating the enemies.
  for ( auto enemy : this->enemies )
  {
    return_objects.clear();
    this -> quadTree->retrieve ( return_objects, enemy->get_bounding_box () );
    enemy -> setCollisionRects ( return_objects );
    enemy -> update ( DELTA_TIME );
  }

  // Set to GameOver if the player is dead.
  if( this -> player -> isDead () )
  {
    Game::instance ().setState ( Game::GStates::GAMEOVER );
    return;
  }

  // Updating the potions.
  for ( auto potion : this -> player->potions )
  {
    return_objects.clear ();
    this -> quadTree -> retrieve ( return_objects, potion->get_bounding_box () );
    potion -> setCollisionRects ( return_objects );
  }

  /// @todo Maybe refactor this static Enemy::px, Enemy::py.
  // Updating player info for the enemies.
  Enemy::px = this -> player -> x;
  Enemy::py = this -> player -> y;
  Enemy::position_vulnerable = this -> player -> is_vulnerable;

  //update number of potions where limit of number itens = 4 for level two. 
  for ( int i = 0; i < NUMBER_ITEMS; ++i )
  { 
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), 
      {items [ 0 ] [ i ], items [ 1 ] [ i ], 192, 192}) && caught_items [ i ] == false )
    {
      this -> player -> addPotions(3);
      caught_items [ i ] =true;
    }
  }

  if ( this -> player -> life != Enemy::points_life )
  {
    if( this -> player -> is_vulnerable )
    {
      this -> player -> life--;
      Enemy::points_life = this -> player -> life;
      this -> player -> changeState ( Player::player_states::HITED );
      this -> player -> is_vulnerable = false;
    }
    else
    {

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
  }

  // Updating the potion/enemy collision.
  for( auto potion : this -> player -> potions )
  {
    for(auto enemy : this -> enemies)
    {
      if( Collision::rects_collided ( potion -> get_bounding_box (), enemy -> get_bounding_box () ) )
      {
        if ( potion -> activated )
        {
          
          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= 100;
            potion -> activated = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;

          if( enemy -> life <= 0 )
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
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), enemy -> get_bounding_box () ) )
    {
      if( this -> player -> is_right != enemy -> is_right )
        if( this -> player -> is_current_state( Player::player_states::ATTACK ) || 
            this -> player -> is_current_state( Player::player_states::ATTACKMOVING ) )
        {
          
          if ( enemy -> life > 0 && this -> player -> can_attack )
          {
            enemy -> life -= this -> player -> attack_strength;
            this -> player -> can_attack = false;
          }
          // Log(DEBUG) << "Enemy Life = " << enemy->life;

          if( enemy -> life <= 0 )
          {
            enemy -> changeState ( Enemy::EStates::DEAD );
          }
        }
    }
  }

  //Saving the game state
  for( int j = 0; j < this -> NUMBER_OF_CHECKPOINTS; ++j )
  {
    if ( !this -> checkpoints_visited [ j ] && this -> player -> get_bounding_box().x >= checkpoints_X [ j ] 
        && this -> player -> get_bounding_box().x <= checkpoints_X [ j ] + 100 &&
            this -> player -> get_bounding_box().y >= checkpoints_Y [ j ]
        && this -> player -> get_bounding_box().y <= checkpoints_Y [ j ] + 200 )
    {
      this -> checkpoints [ j ] = Game::instance ().getResources ().get( "res/images/checkpoint_visited.png" );
      Game::instance ().get_saves ().saveLevel ( 2, this -> player, this -> enemies, Game::instance ().current_slot );
      this -> checkpoints_visited [ j ] = true;
    } 
  }

  // Documents check
  for ( auto document : this -> documents )
  {
    if ( Collision::rects_collided ( this -> player -> get_bounding_box (), document -> get_bounding_box () ) )
    {
      document -> should_render = true;
    }
    else
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
void LevelTwo::render ()
{
  const int CAMERA_X = this -> camera -> getClip ().x;  //Declaring the x position of the camera at level 2.
  const int CAMERA_Y = this -> camera -> getClip ().y;  //Declaring the y position of the camera at level 2.

  this -> background -> render ( 0, 0 );

  for ( int j = 0; j < this -> NUMBER_OF_CHECKPOINTS; ++j )
  {
    this -> checkpoints [ j ] -> render ( this -> checkpoints_X [ j ] - CAMERA_X, this -> checkpoints_Y [ j ] - CAMERA_Y );
  }

  // Render the tiles in the TileMap.
  this -> tile_map -> render( CAMERA_X, CAMERA_Y );

  this -> player_Hud -> render ();

  for( auto enemy : this -> enemies )
  {
    enemy -> render ( CAMERA_X, CAMERA_Y );
  }

  // Render all the entities in the list.
  for( auto entity : this -> entities )
  {
        entity -> render( CAMERA_X, CAMERA_Y );
  }

  for ( unsigned int i = 0; i < NUMBER_ITEMS; i++ )
  {
    if ( this -> image != nullptr && caught_items [ i ] == false )
    {
      
      this -> image -> Sprite::render ( ( items [ 0 ] [ i ]+60 ) - CAMERA_X, ( ( items [ 1 ] [ i ] ) - CAMERA_Y ) );
    }
  }

  // Document text image
  for ( auto document : this -> documents )
  {
    document -> render( CAMERA_X, CAMERA_Y );
    if ( document -> should_render )
    {
      document -> renderDocumentText ();
    }
  }
}
