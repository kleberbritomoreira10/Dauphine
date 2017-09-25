/*
 * Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FadeScreen.h
 * Objects in this class are responsible for determining the behavior of the fade on the screen.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_FADESCREEN_H
#define INCLUDE_FADESCREEN_H

#include "Entity.h"
#include "Sprite.h"
#include "FadeHandler.h"

// Objects in this class are responsible for determining the behavior of the fade on the screen
class FadeScreen
{

	public:
		FadeScreen ();
		virtual ~FadeScreen ();

		/*
		Update the position of fade and when show up or not
		Update is based on what input was recieved and where the character is in the game
		@param dt_ : Delta time. Time elapsed between one frame and the other
		@param percentage_: Percentage of image appearance on screen
		@param time_: Time the image will appear on screen
		*/
		void update ( const double dt_ );
		void render ();
		bool isFaded ();
		void fadeOut ( const double percentage_, const double time_ );
		void fadeIn ( const double percentage_, const double time_ );

	private:

		Sprite *fadeImage;
		FadeHandler *fadeHandler;

};

#endif // INCLUDE_FADESCREEN_H
