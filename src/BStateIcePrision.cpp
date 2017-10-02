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
  this -> boss -> power = Game::instance().getResources().get("res/images/ice_prision.png");
  this -> boss -> power_animation -> changeWidthHeight( 340,1020 );
  this -> boss -> power_animation -> changeAnimation( 0, 0, 2, false, 0.5 );
  this -> boss -> vx = 0;
  this -> boss -> vy = 0;
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
  this -> boss -> player -> isVulnerable = true;
  this -> boss -> power_animation -> changeAnimation( 0, 0, 1, false, 0 );
  prisionTime = 0.0;
  this -> boss -> player -> canMove = true;
}

/**
* Changing the position of the boss in ice prision.
* @param dt_: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void BStateIcePrision::update( const double dt_ )
{
  prisionTime += dt_;
  if( prisionTime > 1 )
  {
    this -> boss -> power_animation -> changeAnimation( 2, 0, 1, false, 0 );
    if( Collision::rectsCollided( this -> boss -> player -> getBoundingBox(),  {( int )this -> boss -> power_X, 
    ( int ) this -> boss -> power_Y, 340,1020 }))
    {
      if( this -> boss -> player -> isVulnerable)
      {
          this -> boss -> player -> canMove = false;
          this -> boss -> player -> isVulnerable = false;
          this -> boss -> player -> vx = 0;
          this -> boss -> player -> vy = 0;
          this -> boss -> player -> getAnimation() -> changeAnimation( 4, 8, 1, false, 0 );
      }
    }
  }
  if( prisionTime > 4 )
  {
    this -> boss -> changeState( Boss::BStates::IDLE );
  }
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIcePrision::BStateIcePrision( Boss* const boss_ ) :
  StateBoss(boss_)
{

}
