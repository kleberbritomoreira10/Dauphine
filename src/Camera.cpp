/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Camera.cpp
* The state of the Camera of the game.
* Class that controll the camera of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "Camera.h"
#include "Configuration.h"

/**
* The constructor.
* Initializes the attributes.
*/
Camera::Camera( Entity *const entity_ ):
  entity( entity_ ),
  level_width( 0 ),
  level_height( 0 ),
  clip{ 0, 0, ( int ) Configuration::getCameraDistanceWidth(), ( int )Configuration::getCameraDistanceHeight()}
{
  /// @todo If the player changes the resolution, so should the clip. (?)
}

/**
* The destructor.
*/
Camera::~Camera()
{

}

/**
* Changing the camera position in the game.
*/
void Camera::update()
{
  updatePosition();
}

SDL_Rect &Camera::getClip()
{
  return this -> clip;
}

/**
* Changing the camera position in the game.
*/
void Camera::updatePosition()
{

  this -> clip.x = ( this -> entity -> x + this -> entity -> getWidth() / 2 ) - ( this -> clip.w / 2 );
  this -> clip.y = ( this -> entity -> y + this -> entity -> getHeight() / 2 ) - (this -> clip.h / 2 );

  // Left wall.
  if( this -> clip.x < 0 )
  {
    this -> clip.x = 0;

  }else if( this -> clip.x > ( int )this -> level_width - this -> clip.w ) // Right wall.
  {
    this -> clip.x = ( int ) this -> level_width - this -> clip.w;

  }else
  {
    // No action.
  }

  // Top wall.
  if( this -> clip.y < 0)
  {
    this -> clip.y = 0;

  }else if( this -> clip.y > ( int ) this -> level_height - this -> clip.h) // Bottom wall.
  {
    this -> clip.y = ( int ) this -> level_height - this -> clip.h;

  }else
  {
    // No action.
  }
}

/**
* Changing the camera position in the game for the center.
* @param entity_: Character of the game that will be in the center.
*/
void Camera::centralizeOn(Entity *const entity_)
{
  this -> entity = entity_;
}

/**
* Changing the camera position in the game based in configured width and height.
* @param width__: Width will be configured in the camera.
* @param height_: Height will be configured in the camera.
*/
void Camera::setLevelWH( const unsigned int width_, const unsigned int height_ )
{
  this -> level_width = width_;
  this -> level_height = height_;
}
