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
Entity::Entity ( const double x_, const double y_, const std::string& PATH ) : x( x_ ), y( y_ ), is_right( true ), sprite(Game::instance().get_resources().get(PATH)), width( 0 ), height( 0 ), animation_clip{ 0, 0, 0, 0 }, 
  bounding_box{ (int)x_, (int)y_, 0, 0 }
{
  // Only serves as the initializer for the derived classes.
  if ( this -> sprite != nullptr )
  {
    this -> width = this -> sprite -> getWidth();
    this -> height = this -> sprite -> getHeight();
    
    this -> bounding_box.w = this -> width;
    this -> bounding_box.h = this -> height;
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
Entity::Entity ( const double x_, const double y_ ) : x( x_ ), y( y_ ), is_right( true ), sprite( nullptr ), 
  width( 0 ), height( 0 ), animation_clip{ 0, 0, 0, 0 }, bounding_box{ ( int )x_, ( int )y_, 0, 0 }
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
* @return The Entity::animation_clip.
*/
SDL_Rect& Entity::get_animation_clip ()
{
	return this -> animation_clip;
}

/**
* @return The Entity::bounding_box.
*/
SDL_Rect& Entity::get_bounding_box ()
{
  return this -> bounding_box;
}
/*
* set rect of collision.
*/
void Entity::set_collision_rects ( const std::vector<CollisionRect>& collisionRects_ )
{
  this -> collisionRects = collisionRects_;   
}
