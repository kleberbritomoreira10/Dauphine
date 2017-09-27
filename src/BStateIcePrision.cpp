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

double prisionTime = 0.0;

/**
* Shows the animation of the boss entering in the state of ice prision.
*/
void BStateIcePrision::enter()
{
  // Log(DEBUG) << "STATE ICE PRISION BOSS";
  this -> boss -> power = Game::instance().get_resources().get("res/images/ice_prision.png");
  this -> boss -> power_animation -> changeWidthHeight( 340,1020 );
  this -> boss -> power_animation -> change_animation( 0, 0, 2, false, 0.5 );
  this -> boss -> velocity_x_axis = 0;
  this -> boss -> velocity_y_axis = 0;
  this -> boss -> power_is_activated = true;
  this -> boss -> power_X = this -> boss -> player -> x - 30; 
  this -> boss -> power_Y = this -> boss -> player -> y - 750;
  this -> boss -> power_flip = SDL_FLIP_NONE;
}

/**
* Ends with the state of ice prision of the boss.
*/
void BStateIcePrision::exit()
{
  this -> boss -> power_is_activated = false;
  this -> boss -> player -> is_vulnerable = true;
  this -> boss -> power_animation -> change_animation( 0, 0, 1, false, 0 );
  prisionTime = 0.0;
  this -> boss -> player -> canMove = true;
}

/**
* Changing the position of the boss in ice prision.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void BStateIcePrision::update( const double DELTA_TIME )
{
  prisionTime += DELTA_TIME;
  if( prisionTime > 1 )
  {
    this -> boss -> power_animation -> change_animation( 2, 0, 1, false, 0 );
    if( Collision::rects_collided( this -> boss -> player -> get_bounding_box(),  {( int )this -> boss -> power_X, 
    ( int ) this -> boss -> power_Y, 340,1020 }))
    {
      if( this -> boss -> player -> is_vulnerable)
      {
          this -> boss -> player -> canMove = false;
          this -> boss -> player -> is_vulnerable = false;
          this -> boss -> player -> velocity_x_axis = 0;
          this -> boss -> player -> velocity_y_axis = 0;
          this -> boss -> player -> getAnimation() -> change_animation( 4, 8, 1, false, 0 );
      }
    }
  }
  if( prisionTime > 4 )
  {
    this -> boss -> change_state( Boss::BStates::IDLE );
  }
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIcePrision::BStateIcePrision( Boss* const BOSS ) :
  StateBoss(BOSS)
{

}
