/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LevelOne.h
 * The first level of the game.
 * Derived from Level class
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_LEVELONE_H
#define INCLUDE_LEVELONE_H

#include "Level.h"
#include "Sprite.h"

#define NUMBER_ITEMS 4

/**
* The first level of the game.
* Derived from Level class.
*/
class LevelOne : public Level
{

	public:
		/**
		* The constructor.
		* @see Level::Level()
		*/
		LevelOne ();

		virtual ~LevelOne ();

		/**
		* Loads the level.
		* From the Level1.lua script, loads all the necessary objects.
		*/
		virtual void load ();

		/**
		* Updates the objects within the Level.
		* @param dt_ : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double dt_ );

		/**
		* Unloads everything that was loaded.
		* @see LevelOne::load()
		*/
		virtual void unload ();

		/**
		* Renders the level.
		* Always renders on 0,0 position.
		* @see Sprite::render()
		*/
		virtual void render ();

		Sprite *image; // the image attributed to the Sprite
		int items [ 2 ] [ NUMBER_ITEMS ]; // upper index of items on the game
		bool caughtItems [ NUMBER_ITEMS ]; // boolean for the number of items that were caught or not

};

#endif //INCLUDE_LEVELONE_H
