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

#define WIDTH_VALUE 50
#define HEIGHT_VALUE 50
#define TOTAL_TIME 0.5
#define NUMBER_OF_IMAGE 4
#define REACH_PROJECTILE 15
#define ANIMATION_POSITION_X 0
#define ANIMATION_POSITION_Y 0

double leg_x_axis = 0;  //Declaration variable for side x of the cateto.
double leg_y_axis = 0;  //Declaration variable for side y of the cateto.
double hypotenuse = 0;  //Declaration variable for hypotenuse calculation.

/**
* @see StateBoss::enter
*/
void BStateMagicProjectile::enter ()
{
  Log(DEBUG) << "STATE MAGIC PROJECTILE BOSS";
  this -> boss -> power = Game::instance().getResources().get( "res/images/projectile.png" );
  this -> boss -> power_animation -> changeWidthHeight( WIDTH_VALUE, HEIGHT_VALUE );
  this -> boss -> power_animation -> changeAnimation( ANIMATION_POSITION_X, ANIMATION_POSITION_Y, NUMBER_OF_IMAGE, false, TOTAL_TIME );
  this -> boss -> velocity_x_axis = 0;
  this -> boss -> velocity_y_axis = 0;
  this -> boss -> power_X_axis = this -> boss -> x + 50;
  this -> boss -> power_Y_axis = this -> boss -> y + 150;

  leg_x_axis = this -> boss -> player -> get_bounding_box().x + WIDTH_VALUE - this -> boss -> get_bounding_box().x;
  leg_y_axis = this -> boss -> player -> get_bounding_box().y + HEIGHT_VALUE - this -> boss -> get_bounding_box().y;
  hypotenuse = sqrt( ( leg_x_axis * leg_x_axis ) + ( leg_y_axis * leg_y_axis ) );
  hypotenuse = ( hypotenuse == 0 ) ? 1 : hypotenuse;
  leg_x_axis /= hypotenuse;
  leg_y_axis /= hypotenuse;
}

double projectileTime = 0.0;  //Declaring variable that stores the launch time of the projectile executed by the boss.

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
  this -> boss -> power_animation -> changeWidthHeight( WIDTH_VALUE, HEIGHT_VALUE );
}

/**
* @see StateBoss::update 
*/
void BStateMagicProjectile::update ( const double DELTA_TIME ) 
{ 
  assert(DELTA_TIME > 0);
  projectileTime += DELTA_TIME;
  this -> boss -> power_X_axis += leg_x_axis * REACH_PROJECTILE;
  this -> boss -> power_Y_axis += leg_y_axis * REACH_PROJECTILE;
  this -> boss -> power_is_activated = true;

  if ( Collision::rects_collided( this -> boss -> player -> get_bounding_box(), { ( int )this -> boss -> power_X_axis, 
    (int)this -> boss -> power_Y_axis, WIDTH_VALUE, HEIGHT_VALUE } ) )
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
