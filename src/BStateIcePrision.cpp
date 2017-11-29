/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateIcePrision.cpp
* The state of the ice prision of the boss.
* Class that controll the ice prision of the boss.
* License: Copyright (C) 2014 Alke Games.
*/

#include "BStateIcePrision.h"
#include "Logger.h"
#include "Sprite.h"
#include "Game.h"
#include "Collision.h"
#include <assert.h>

#define WIDTH_VALUE 340
#define HEIGHT_VALUE 1020
#define ANIMATION_POSITION_X 0
#define ANIMATION_POSITION_Y 0
#define ANIMATION_TOTAL_TIME 0


/**
* Shows the animation of the boss entering in the state of ice prision.
*/
void BStateIcePrision::enter()
{
   Log( INFO ) << "STATE ICE PRISION BOSS";

  // Load the boss Ice Prision Image
  this -> boss -> power = Game::instance().getResources().get( "res/images/ice_prision.png" ); 
  this -> boss -> power_animation -> changeWidthHeight( WIDTH_VALUE, HEIGHT_VALUE ); // Change animation width and Height.

  // Change IcePrision animation.
  this -> boss -> power_animation -> changeAnimation( ANIMATION_POSITION_X, ANIMATION_POSITION_X, 2, false, 0.5 );
  this -> boss -> velocity_x_axis = 0;
  this -> boss -> velocity_y_axis = 0;
  this -> boss -> power_is_activated = true;
  this -> boss -> power_X_axis = this -> boss -> player -> x - 30;
  this -> boss -> power_Y_axis = this -> boss -> player -> y - 750;
  this -> boss -> power_flip = SDL_FLIP_NONE;
}

/**
* Ends with the state of ice prision of the boss.
*/
void BStateIcePrision::exit()
{
  Log( INFO ) << "Exiting Ice Prision state.";

  this -> boss -> power_is_activated = false;
  this -> boss -> player -> is_vulnerable = true;

  // Changes the power animation.
  this -> boss -> power_animation -> changeAnimation( ANIMATION_POSITION_X, ANIMATION_POSITION_Y, 1, 
                                                    false, ANIMATION_TOTAL_TIME );
  prision_time = 0.0;
  this -> boss -> player -> can_move = true;
}

/**
* Changing the position of the boss in ice prision.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void BStateIcePrision::update( const double DELTA_TIME )
{
  assert( DELTA_TIME > 0 ); // Check if the if time variation.
  prision_time += DELTA_TIME;

  if( prision_time > 1 )
  {
    // Changes the ice prision animation.
    this -> boss -> power_animation -> changeAnimation( 2, ANIMATION_POSITION_Y, 1, false, ANIMATION_TOTAL_TIME );

    update_player();
    
  }
  if( prision_time > 4 )
  {

    this -> boss -> changeState( Boss::BStates::IDLE );

  }else
  {
    Log( DEBUG ) << "Invalid prision time.";
  }

}

// Update all the player characteristcs when state ice prision.
void BStateIcePrision::update_player()
{
  if( Collision::rects_collided( this -> boss -> player -> get_bounding_box(),  { ( int )this -> boss -> power_X_axis,
  ( int ) this -> boss -> power_Y_axis, WIDTH_VALUE, HEIGHT_VALUE }))
  {
    if( this -> boss -> player -> is_vulnerable)
    {

        this -> boss -> player -> can_move = false;
        this -> boss -> player -> is_vulnerable = false;
        this -> boss -> player -> velocity_x_axis = 0;
        this -> boss -> player -> velocity_y_axis = 0;
        this -> boss -> player -> getAnimation() -> changeAnimation( 4, 8, 1, false, 0 ); // Changes the ice prision animation.

    }else
    {
      Log( DEBUG ) << "Player is not vulnerable.";
    }

  }else
  {
    Log( DEBUG ) << "Invalid rects!";
  }
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIcePrision::BStateIcePrision( Boss *const BOSS ) :
  StateBoss(BOSS)
{

}
