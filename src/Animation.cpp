/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Animation.cpp
 * Class in control of animating a sprite.
 */

#include "Animation.h"
#include <assert.h>
#include <cstddef>

#define START_X_POSITION 0
#define START_Y_POSITION 0
#define ZERO 0.0

/**
 * The constructor.
 * Initializes all attributes.
 * @param x_ : The x position on the spritesheet.
 * @param y_ : The y position on the spritesheet.
 * @param spriteWidth_ : The width of the sprite to animate.
 * @param spriteHeight_ : The height of the sprite to animate.
 * @param numberOfImages_ : The number of images to animate inside the spritesheet.
 * @param loop_ : Whether to loop or not.
 */
Animation::Animation ( const int x_, const int y_, const int spriteWidth_, const int spriteHeight_, 
	const unsigned int numberOfImages_, const bool loop_ ) : ANIMATION_LIMIT( 10 ), x( x_ ), y( y_ ), initial_x( START_X_POSITION ), initial_y( START_Y_POSITION ), sprite_width( spriteWidth_ ), sprite_height( spriteHeight_ ), loop( loop_ ), totalElapsedTime( ZERO ), totalTime( ZERO )
{
	this -> numberOfImages = (numberOfImages_ == 0) ? 1 : numberOfImages_;
}

/**
* The destructor.
*/
Animation::~Animation()
{

}

int animationCount = 0;  //Declaring auxiliary variable for the repeat structure.

/**
* Updates the animation clip.
* @param clip_ : Reference to the clip of whatever object the animation is a part of.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent of processing speed.
* @param totalTime_ : How much time each frame should have. Affects the speed on which the animation changes.
*/
void Animation::update ( SDL_Rect& clip, const double DELTA_TIME )
{	
	assert( DELTA_TIME > 0 );
	// Compare the position on the sprite with the number of positions to know if is the end of the animation.
	bool endOfAnimation = ( ( animationCount + 1 ) >= this -> numberOfImages );

	// Time total between one frame and the other
	const double DELTA_TOTAL = ( this -> totalTime / this -> numberOfImages );  

	// Check if the frame has changed.
  this -> totalElapsedTime += DELTA_TIME;

  if ( this -> totalElapsedTime >= DELTA_TOTAL )
  {    
    this -> totalElapsedTime = 0;
    animationCount += 1;

    if ( Animation::animationCount <= this -> numberOfImages )
    {
      if ( this -> x < ( int )ANIMATION_LIMIT )
      {
        this -> x += 1;
      } else
        {
        	this -> y += 1;
        	this -> x = 0;
        }  
    } else
    	{
        //Nothing to do 
    	}

    if ( endOfAnimation )
    {
    	this -> x= this -> initial_x;
    	this -> y= this -> initial_y;
    	Animation::animationCount = 0;
    } else
    	{
        //Nothing to do 
    	}
  } else
    {
      //Nothing to do 
    }

  const int POSITION_X = this -> x * this -> sprite_width;  //Defining width animation
  const int POSITION_Y = this -> y * this -> sprite_height; //Defining height animation
  updateClip( clip, POSITION_X, POSITION_Y );
}

/**
* Changes the animation to another.
* @note See Animation::Animation for the parameters descriptions.
*/
void Animation::changeAnimation ( const int x_, const int y_, const unsigned int numberOfImages_,
	const bool loop_, const double totalTime_ )
{
  //assert( totalTime_ > 0 );
	this -> x = x_;
	this -> y = y_;
	this -> initial_x = x_;
	this -> initial_y = y_;
	this -> numberOfImages = ( numberOfImages_ == 0 ) ? 1 : numberOfImages_;
	this -> loop = loop_;
	this -> totalTime = totalTime_;
	animationCount = 0;
}

/**
* Updates the clip to a new position.
* @param clip_ : The clip to update.
* @param x_ : New x position.
* @param y_ : New y position.
*/
void Animation::updateClip ( SDL_Rect& clip, const int x_, const int y_ )
{ 
	clip.x = x_;
	clip.y = y_;
	clip.w = this -> sprite_width;
	clip.h = this -> sprite_height;
}

/*
* Increment current frame
* @return current frame incremented in one
*/
int Animation::getCurrentFrame ()
{
	return animationCount + 1;
}

/*
* change sprite position
* @param width_ : width position
* @param height_ : height position
*/
void Animation::changeWidthHeight ( int width_, int height_ )
{
	assert( width_ > 0 || height_ > 0 );
	this -> sprite_width = width_;
	this -> sprite_height = height_;
}
