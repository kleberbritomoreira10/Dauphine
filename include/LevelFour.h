/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LevelFour.h
 * File responsible for implementing the level 4 features of the game. Using for this the virtual is 
 * a function or method whose behavior can be overwritten in an inheriting class by a function with the
 * same signature. 
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_LEVELFOUR_H
#define INCLUDE_LEVELFOUR_H

#include "Level.h"
#include "Sprite.h"

#define NUMBER_ITEMS 4

/*
 * The first level of the game.
 * Derived from Level class.
 */
class LevelFour : public Level 
{
	
	public:
		/**
		* The constructor.
		* @see Level::Level()
		*/
		LevelFour ();

		/**
		* The destructor.
		*/
		virtual ~LevelFour ();

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
		* @see LevelFour::load()
		*/
		virtual void unload ();

		/**
		* Renders the level.
		* Always renders on 0,0 position.
		* @see Sprite::render()
		*/
		virtual void render ();

		int items [ 2 ] [ NUMBER_ITEMS ];
		Sprite *image;
		bool caughtItems [ NUMBER_ITEMS ];

};

#endif //INCLUDE_LEVELFOUR_H