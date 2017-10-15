#ifndef INCLUDE_GSTATECONTNUE_H
#define INCLUDE_GSTATECONTINUE_H

#include "StateGame.h"
#include "Sprite.h"
#include "Text.h"

/**
* The state for the Continue menu screen.
*/
class GStateContinue : public StateGame 
{

	public:
		/**
		* The constructor.
		* Initializes all the attributes.
		*/
		GStateContinue ();

		/**
		* The destructor.
		*/
		virtual ~GStateContinue ();

		/**
		* Loads the level.
		* From the menu.lua script, loads all the necessary objects.
		*/
		virtual void load ();

		/**
		* Updates the objects within the StateGame.
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double DELTA_TIME );

		/**
		* Unloads everything that was loaded.
		* @see GStateContinue::load
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

			SLOT_1 = 0,
			SLOT_2,
			SLOT_3,
			TOTAL
		};

		Sprite *background; /**< Background image. */
		Sprite *selector; /**< Image for the slot selector. */
		double passed_time; /**< The time already elapsed since the beggining of the menu. */

		int current_selection;
		int selectorXPosition; /**< The X position of the slot selector. */
		int selectorYPosition [ Selection::TOTAL ]; /**< The Y position of the slot selector. */

		Text *slot1;
		Text *slot2;
		Text *slot3;
};

#endif // INCLUDE_GSTATEMENU_H