/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Animation.cpp
* Class in control of animating a sprite.
*/

#include "Animation.h"

int animationCount = 0;

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
Animation::Animation ( const int x_, const int y_, const int spriteWidth_,
	const int spriteHeight_, const unsigned int numberOfImages_, const bool loop_ ) :
	
	ANIMATION_LIMIT( 10 ),
	x( x_ ),
	y( y_ ),
	initialX( 0 ),
	initialY( 0 ),
	spriteWidth( spriteWidth_ ),
	spriteHeight( spriteHeight_ ),
	loop( loop_ ),
	totalElapsedTime( 0.0 ),
	totalTime( 0.0 )
{
	this -> numberOfImages = (numberOfImages_ == 0) ? 1 : numberOfImages_;
}

/**
* The destructor.
*/
Animation::~Animation()
{

}

/**
* Updates the animation clip.
* @param clip_ : Reference to the clip of whatever object the animation is a part of.
* @param dt_ : Delta time. Time elapsed between one frame and the other, independent of processing speed.
* @param totalTime_ : How much time each frame should have. Affects the speed on which the animation changes.
*/
void Animation::update ( SDL_Rect& clip, const double dt_ )
{
	// Compare the position on the sprite with the number of positions to know if is the
	// end of the animation.
	bool endOfAnimation = ( ( animationCount + 1 ) >= this -> numberOfImages );

	const double deltaT = ( this -> totalTime / this -> numberOfImages );

	// Check if the frame has changed.
    this -> totalElapsedTime += dt_;

    if ( this -> totalElapsedTime >= deltaT )
    {    
      this -> totalElapsedTime = 0;
      animationCount += 1;

      if ( Animation::animationCount <= this -> numberOfImages )
      {
        if ( this -> x < ( int )ANIMATION_LIMIT )
        {
        	this -> x += 1;
        }
        else
        {
        	this -> y += 1;
        	this -> x = 0;
        }
    	}

    	if ( endOfAnimation )
      {
    		this -> x= this -> initialX;
    		this -> y= this -> initialY;
    		Animation::animationCount = 0;
    	}
    }

   	const int positionX_ = this -> x * this -> spriteWidth;
  	const int positionY_ = this -> y * this -> spriteHeight;
  	updateClip( clip, positionX_, positionY_ );
}

/**
* Changes the animation to another.
* @note See Animation::Animation for the parameters descriptions.
*/
void Animation::changeAnimation ( const int x_, const int y_, const unsigned int numberOfImages_,
	const bool loop_, const double totalTime_ )
{

	this -> x = x_;
	this -> y = y_;
	this -> initialX = x_;
	this -> initialY = y_;
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
	clip.w = this -> spriteWidth;
	clip.h = this -> spriteHeight;
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
* @param width_  : width position
* @param height_ : height position
*/
void Animation::changeWidthHeight ( int width_, int height_ )
{
	this -> spriteWidth = width_;
	this -> spriteHeight = height_;
}
