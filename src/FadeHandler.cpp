/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FadeHandler.cpp
 * File responsible for implementing fade type.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "FadeHandler.h"
#include "Logger.h"

/* 
 * The constructor
 * @param sprite_ : representation for animation
 * @see StateEnemy::exit
 */
FadeHandler::FadeHandler(Sprite* const sprite_) :
	shouldFadeIn(false),
	shouldFadeOut(false),
	percentageOfStop(0.0),
	sprite(sprite_),
	time(0.0),
	currentPercentage(0.0),
	rate(0.0)
{
	this -> currentPercentage = ( this -> sprite->getAlpha()/255.0 );
}

/* 
 * The constructor
 */
FadeHandler::~FadeHandler() 
{
	this -> sprite = nullptr;
}

/* 
 * Manipulate input characters on screen
 * @param percentage : percentage fade in
 * @param time: time fade in
 */
void FadeHandler::fadeIn( const double percentage_, const double time_)
{
	this -> shouldFadeIn = true;
	this -> shouldFadeOut = false;
	this -> percentageOfStop = percentage_;
	this -> currentPercentage = ( this -> sprite->getAlpha()/255.0);
	this -> rate = ( this -> percentageOfStop - this -> currentPercentage)/time_;
}

/* 
 * Manipulate output characters on screen
 * @param percentage : percentage fade out
 * @param time: time fade out
 */
void FadeHandler::fadeOut( const double percentage_, const double time_)
{
	this -> shouldFadeOut = true;
	this -> shouldFadeIn = false;
	this -> percentageOfStop = percentage_;
	this -> currentPercentage = ( this -> sprite->getAlpha()/255.0);
	this -> rate = ( this -> currentPercentage - this -> percentageOfStop)/time_;
}

/* 
 * Manipulate update characters on screen
 * @param dt_ : delta time (time elapsed)
 */
void FadeHandler::update( const double dt_)
{
	if ( this -> sprite == nullptr )
	{
		Log(WARN) << "No sprite set to Fade Handler.";
		return;
	}
  
  //Check the rate that should fade at the input
	if ( shouldFadeIn )
	{
		if ( this -> currentPercentage < this -> percentageOfStop )
		{
			this -> currentPercentage += this -> rate * dt_/1000.0;
			this -> sprite->setAlpha( 255.0 * this -> currentPercentage );
		} else {
			  shouldFadeIn = false;
			  this -> currentPercentage = this -> percentageOfStop;
			  this -> sprite -> setAlpha( 255.0 * this -> currentPercentage );
		  }
	}
	//check the rate that should fade at the output
	else if ( shouldFadeOut )
	{
		if ( this -> currentPercentage > this -> percentageOfStop )
		{
			this -> currentPercentage -= this -> rate * dt_/1000.0;
			this -> sprite -> setAlpha( 255.0 * this -> currentPercentage );
		} else {
			  shouldFadeOut = false;
			  this -> currentPercentage = this -> percentageOfStop;
			  this -> sprite -> setAlpha(255.0 * this -> currentPercentage);
		  }
	}
}

/* 
 * Get current percentage 
 */
double FadeHandler::getCurrentPercentage()
{
	return this -> currentPercentage;
}
