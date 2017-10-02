/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateMagicProtectile.cpp
* State of battle of boss.
*/

#include "BStateMagicProjectile.h"
#include "Logger.h"
#include "Sprite.h"
#include "Game.h"
#include "Collision.h"
#include <cmath>

double projectileTime = 0.0;
double mpX = 0;
double mpY = 0;
double hypotenuse = 0;

/**
* @see StateBoss::enter
*/
void BStateMagicProjectile::enter ()
{
  // Log(DEBUG) << "STATE MAGIC PROJECTILE BOSS";
  this -> boss -> power = Game::instance().getResources().get( "res/images/projectile.png" );
  this -> boss -> power_animation -> changeWidthHeight( 50, 50 );
  this -> boss -> power_animation -> changeAnimation( 0, 0, 4, false, 0.5 );
  this -> boss -> vx = 0;
  this -> boss -> vy = 0;
  this -> boss -> power_X = this -> boss -> x + 50;
  this -> boss -> power_Y = this -> boss -> y + 150;

  mpX = this -> boss -> player -> getBoundingBox().x + 50 - this -> boss -> getBoundingBox().x;
  mpY = this -> boss -> player -> getBoundingBox().y + 50 - this -> boss -> getBoundingBox().y;
  hypotenuse = sqrt( ( mpX * mpX ) + ( mpY * mpY ) );
  hypotenuse = ( hypotenuse == 0 ) ? 1 : hypotenuse;
  mpX /= hypotenuse;
  mpY /= hypotenuse;
}

/**
* @see StateBoss::exit
*/
void BStateMagicProjectile::exit ()
{
  this -> boss -> power_is_activated = false;
  projectileTime = 0.0;
  mpX = 0;
  mpY = 0;
  hypotenuse = 0;
  this -> boss -> power_animation -> changeWidthHeight( 50, 50 );
}

/**
* @see StateBoss::update 
*/
void BStateMagicProjectile::update ( const double dt_ ) 
{
  projectileTime += dt_;
  this -> boss -> power_X += mpX * 15;
  this -> boss -> power_Y += mpY * 15;
  this -> boss -> power_is_activated = true;

  if ( Collision::rectsCollided( this -> boss -> player -> getBoundingBox(), { ( int )this -> boss -> power_X, 
    (int)this -> boss -> power_Y, 50, 50 } ) )
  {
    this -> boss -> player -> life--;
    this -> boss -> changeState( Boss::BStates::IDLE );
  }
  
  if ( projectileTime > 3 )
  {
    this -> boss -> changeState( Boss::BStates::IDLE );
  }
}

/**
* The constructor.
* @param boss_ : Reference to the Boss.
*/
BStateMagicProjectile::BStateMagicProjectile( Boss* const boss_ ) :
  StateBoss( boss_ )
{

}
