/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Potion.cpp
* Class for the bomb potion.
* License: Copyright (C) 2014 Alke Games.*/

#include "Potion.h"
#include "Logger.h"
#include "Window.h"
#include <cmath>

/**
* The constructor.
* @param x_: The x position of de potion.
* @param y_: The y position of the potion.
* @param strength_: The strength of the potion.
* @param inertia_: The inertia of the potion.
* @param distance_: The distance of the potion.
* @param isRight_: Boolean param to indicate if the potion is right.
*/
Potion::Potion( const double x_, const double y_, const std::string &path_, const int strength_, const int inertia_,
	const int distance_, const bool isRight_ ) :

	DynamicEntity( x_, y_, path_ ),
	activated( true ),
  canExplode( true ),
  isExploding( true ),
	strength( strength_ ),
	distance( distance_ ),
	flightTime( 0.0 ),
  animation( nullptr )
{

  this -> width = 192;
	this -> isRight = isRight_;

  // Loading potion width.
	if ( this -> isRight )
	{
		this -> x -= this -> width;
	} else
	{
        this -> x -= this -> width;
  }

    // Loading animation and potion characteristics.
    this -> animation = new Animation( 0, 0, 192, 192, 13, false );
    this -> y = this -> y + 100;
    this -> vx = 5 + abs( inertia_/80 );
    this -> vy = 5;
}


/**
* The destructor.
*/
Potion::~Potion()
{

}

/**
* Update the potions.
* @param dt_: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void Potion::update( const double dt_ )
{

	const int angle = 360 - 45; // Potion animation angle.
	const double gravity = 35; // Gravity value.

  // Updating Bounding box and animation.
  updateBoundingBox();
  this -> animation -> update( this -> animationClip, dt_ );

  // Detecting collisions and handling.
	const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detectCollision();
  handleCollision( detections );

  // Updating all the potion characteristics.
	if ( this -> activated )
	{

    this -> getAnimation() -> changeAnimation( 0, 0, 1, false, 0 );

		this -> flightTime +=dt_;

    // Calculating potion's speed in Y axis and in X axis.
		const double speedXIdk = ( this -> distance/300.0 )*( this -> vx + this -> strength * cos( angle/57.29 ) * flightTime );
		const double speedYIdk = ( this -> vy + this -> strength * sin( angle/57.29 ) *
															flightTime - 0.5*gravity*flightTime*flightTime );

		if ( this -> isRight )
		{
			this -> x += speedXIdk;
		}
		else{
			this -> x -= speedXIdk;
		}

		this -> y -= speedYIdk;
	} else
	{
        // Changing animation if can explode.
        if ( this -> canExplode )
				{
            this -> getAnimation() -> changeAnimation( 1, 0, 12, false, 0.8 );
            this -> canExplode = false;
        }
        if ( this -> getAnimation() -> getCurrentFrame() == 12 )
				{
            this -> isExploding =false;
        }
    }
}


/**
* Handle the detected collisions.
* @param detections_: Array of detected collisions.
*/
void Potion::handleCollision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_ )
{

  // Handling SOLID_TOP collision.
  if ( detections_.at( CollisionSide::SOLID_TOP ) )
	{
		if ( ( int )this -> y%64 > 0 )
		{
    	this -> y += 64 -( int )this -> y%64 + 1;
     	this -> strength = 0.0;
     	this -> activated = false;
    }
	}

  // Handling SOLID_BOTOM collision.
	if ( detections_.at( CollisionSide::SOLID_BOTTOM ) )
	{
  	this -> strength = 0.0;
  	this -> activated = false;
  }

  // Handling SOLID_LEFT collision.
  if ( detections_.at( CollisionSide::SOLID_LEFT ) )
	{
		this -> x -= ( int )( this -> x + this -> width )%64 + 1;
    this -> strength = 0.0;
    this -> activated = false;
  }

  // Handling SOLID_RIGHT collision.
  if ( detections_.at( CollisionSide::SOLID_RIGHT ) )
	{
    if ( ( int )this -> x%64 > 0 )
		{
     	this -> x += ( 64 - ( int )this -> x%64 ) + 1;
     	this -> vx = 0.0;
    }
    this -> activated = false;
  }
}


/**
* Renders the potion.
* Uses the player's sprite render method.
* @see Sprite::render
* @param cameraX_ : The x position of the camera.
* @param cameraY_ : The y position of the camera.
*/
void Potion::render( const double cameraX_, const double cameraY_ )
{

	const double dx = this -> x - cameraX_ + this -> width - 64; // Potion's position in X axis.
  const double dy = this -> y - cameraY_ - this -> height; // Potion's position in Y axis.

  /*Actual.
 SDL_Rect actualRect = {( int )dx, ( int )dy, ( int )this -> width, ( int )this -> height};
 SDL_SetRenderDrawColor(  Window::getRenderer(), 0x00, 0x00, 0x00, 0xFF );
 SDL_RenderFillRect( Window::getRenderer(), &actualRect );
 Bounding box.
 SDL_Rect boundingBox2 = {( int )( this -> boundingBox.x - cameraX_ ), ( int )( this -> boundingBox.y - cameraY_ ), ( int )this -> boundingBox.w, ( int )this -> boundingBox.h};
 SDL_SetRenderDrawColor(  Window::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
 SDL_RenderFillRect( Window::getRenderer(), &boundingBox2 );*/

  // Rendering sprite if it is not null and if it is exploding.
  if ( this -> sprite != nullptr && this -> isExploding )
	{
		this -> sprite -> render( dx, dy, &this -> animationClip, false, this -> vx * 3/2, nullptr, SDL_FLIP_HORIZONTAL );
  }
}

/**
* Update the bounding box.
*/
void Potion::updateBoundingBox()
{
  this -> boundingBox.x = ( int ) this -> x + this -> width;
  this -> boundingBox.y = ( int ) this -> y - 32;
  this -> boundingBox.w = ( int ) 32;
  this -> boundingBox.h = ( int ) 32;
}

/**
* Get the potion's animation.
* @return: Return the animation.
*/
Animation *Potion::getAnimation()
{
	return ( this -> animation );
}
