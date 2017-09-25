/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Text.h
 * The state of the text in the game.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_TEXT_H
#define INCLUDE_TEXT_H

#include "SDLWrapper.h"
#include "Entity.h"

class Text : public Entity
{

	public:
		Text( const double x_, const double y_, const char *path_, const int size_,
			const char *text_, const SDL_Color color_ );

		Text( const double x_, const double y_, const char *path_, const int size_,
			const char *text_ );

		virtual ~Text();

		/**
		* Updates the text.
		* @param dt_ : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update( const double dt_ );

		/**
		* Renders the text.
		* @param cameraX_ : The x position of the camera.
		* @param cameraY_ : The y position of the camera.
		*/
		virtual void render( const double cameraX_, const double cameraY_ );

		virtual void changeText( const char *text_, const SDL_Color color_ );
		virtual void changeText( const char *text_ );

	private:
		TTF_Font *font;

};
#endif //INCLUDE_TEXT_H
