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
#include <assert.h>
#include <cstddef>

double projectileTime = 0.0;  //Declaring variable that stores the launch time of the projectile executed by the boss.
double leg_x_axis = 0;  //Declaration variable for side x of the cateto.
double leg_y_axis = 0;  //Declaration variable for side y of the cateto.
double hypotenuse = 0;  //Declaration variable for hypotenuse calculation.

/**
* @see StateBoss::enter
*/
void BStateMagicProjectile::enter ()
{
  // Log(DEBUG) << "STATE MAGIC PROJECTILE BOSS";
  this -> boss -> power = Game::instance().getResources().get( "res/images/projectile.png" );
  this -> boss -> power_animation -> changeWidthHeight( 50, 50 );
  this -> boss -> power_animation -> changeAnimation( 0, 0, 4, false, 0.5 );
  this -> boss -> velocity_x_axis = 0;
  this -> boss -> velocity_y_axis = 0;
  this -> boss -> power_X_axis = this -> boss -> x + 50;
  this -> boss -> power_Y_axis = this -> boss -> y + 150;

  leg_x_axis = this -> boss -> player -> get_bounding_box().x + 50 - this -> boss -> get_bounding_box().x;
  leg_y_axis = this -> boss -> player -> get_bounding_box().y + 50 - this -> boss -> get_bounding_box().y;
  hypotenuse = sqrt( ( leg_x_axis * leg_x_axis ) + ( leg_y_axis * leg_y_axis ) );
  hypotenuse = ( hypotenuse == 0 ) ? 1 : hypotenuse;
  leg_x_axis /= hypotenuse;
  leg_y_axis /= hypotenuse;
}

/**
* @see StateBoss::exit
*/
void BStateMagicProjectile::exit ()
{
  this -> boss -> power_is_activated = false; 
  projectileTime = 0.0; 
  leg_x_axis = 0; 
  leg_y_axis = 0; 
  hypotenuse = 0;
  this -> boss -> power_animation -> changeWidthHeight( 50, 50 );
}

/**
* @see StateBoss::update 
*/
void BStateMagicProjectile::update ( const double DELTA_TIME ) 
{ 
  assert(DELTA_TIME < 0);
  projectileTime += DELTA_TIME;
  this -> boss -> power_X_axis += leg_x_axis * 15;
  this -> boss -> power_Y_axis += leg_y_axis * 15;
  this -> boss -> power_is_activated = true;

  if ( Collision::rects_collided( this -> boss -> player -> get_bounding_box(), { ( int )this -> boss -> power_X_axis, 
    (int)this -> boss -> power_Y_axis, 50, 50 } ) )
  {
    this -> boss -> player -> life--;
    this -> boss -> changeState( Boss::BStates::IDLE );
  } else
    {
      //Nothing to do
    }
  
  if ( projectileTime > 3 )
  {
    this -> boss -> changeState( Boss::BStates::IDLE );
  } else
    {
      //Nothing to do
    }
}

/**
* The constructor.
* @param BOSS : Reference to the Boss.
*/
BStateMagicProjectile::BStateMagicProjectile( Boss* const BOSS ) : StateBoss( BOSS )
{

}
