/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @GStateNewGame.h
 * The state for the Continue menu screen.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_GSTATENEWGAME_H
#define INCLUDE_GSTATENEWGAME_H

#include "StateGame.h"
#include "Sprite.h"
#include "Text.h"

/**
* The state for the Continue menu screen.
*/
class GStateNewGame : public StateGame
{

	public:
		/**
		* The constructor.
		* Initializes all the attributes.
		*/
		GStateNewGame ();

		/**
		* The destructor.
		*/
		virtual ~GStateNewGame ();

		/**
		* Loads the level.
		* From the menu.lua script, loads all the necessary objects.
		*/
		virtual void load ();

		/**
		* Updates the objects within the StateGame.
		* @param dt_ : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double dt_ );

		/**
		* Unloads everything that was loaded.
		* @see GStateNewGame::load
		*/
		virtual void unload ();

		/**
		* Renders the state.
		* Always renders on 0,0 position.
		* @see Sprite::render
		*/
		virtual void render ();

		void handleSelectorMenu ();

	private:
		enum Selection : uint8_t
		{

			SLOT_1,
			SLOT_2,
			SLOT_3,
			TOTAL

		};

		Sprite *background; // Background image.
		Sprite *selector; // Image for the slot selector.
		double passedTime; // The time already elapsed since the beggining of the menu.

		int currentSelection;
		int selectorXPosition; // The X position of the slot selector.
		int selectorYPosition [ Selection::TOTAL ]; // The Y position of the slot selector.

		Text *slot1; // Reference to slot 1 text and font
		Text *slot2; // Reference to slot 2 text and font
		Text *slot3; // Reference to slot 3 text and font
};

#endif // INCLUDE_GSTATEMENU_H
