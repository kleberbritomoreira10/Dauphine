/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Entity.cpp
* Base class for all game objects (entities).
* The entity contains the (x,y) position, width and height, and a sprite.
*/

#include "Entity.h"
#include "Game.h"
#include "SDLWrapper.h"
#include "Logger.h"

/**
* The constructor.
* Initializes all the attributes.
* @param x_ : position in x axis.
* @param y_ : position in y axis.
* @param sprite_ : which sprite to use.
*/
Entity::Entity ( const double x_, const double y_, const std::string& path_ ) :
    x( x_ ),
    y( y_ ),
    isRight( true ),
    sprite(Game::instance().getResources().get(path_)),
    width( 0 ),
    height( 0 ),
    animationClip{ 0, 0, 0, 0 },
    boundingBox{ (int)x_, (int)y_, 0, 0 }
{
    // Only serves as the initializer for the derived classes.

  if ( this -> sprite != nullptr )
  {
    this -> width = this -> sprite -> getWidth();
    this -> height = this -> sprite -> getHeight();
    
    this -> boundingBox.w = this -> width;
    this -> boundingBox.h = this -> height;
  }

  else
  {
      Log(WARN) << "Entity sprite is null, width and height will be undefined.";
  }
}

/**
* The constructor.
* Initializes all the attributes.
* @param x_ : position in x axis.
* @param y_ : position in y axis.
*/
Entity::Entity ( const double x_, const double y_ ) :
  x( x_ ),
  y( y_ ),
  isRight( true ),
  sprite( nullptr ),
  width( 0 ),
  height( 0 ),
  animationClip{ 0, 0, 0, 0 },
  boundingBox{ ( int )x_, ( int )y_, 0, 0 }
{
    // Only serves as the initializer for the derived classes.
}

/**
* The destructor.
*/
Entity::~Entity ()
{

}

/**
* @return The Entity width.
*/
unsigned int Entity::getWidth ()
{
	return this -> width;
}

/**
* @return The Entity height.
*/
unsigned int Entity::getHeight ()
{
	return this -> height;
}

/**
* @return The Entity::animationClip.
*/
SDL_Rect& Entity::getAnimationClip ()
{
	return this -> animationClip;
}

/**
* @return The Entity::boundingBox.
*/
SDL_Rect& Entity::getBoundingBox ()
{
  return this -> boundingBox;
}
/*
* set rect of collision.
*/
void Entity::setCollisionRects ( const std::vector<CollisionRect>& collisionRects_ )
{
  this -> collisionRects = collisionRects_;   
}
