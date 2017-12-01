#ifndef INCLUDE_GSTATEMENU_H
#define INCLUDE_GSTATEMENU_H

#include "StateGame.h"
#include "Sprite.h"
#include "Animation.h"

/**
* The state for the initial menu screen.
* Game state that will contain the main menu.
*/
class GStateMenu : public StateGame
{

	public:
		/**
		* The constructor.
		* Initializes all the attributes.
		*/
		GStateMenu ();

		/**
		* The destructor.
		*/
		virtual ~GStateMenu ();

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
		* @see GStateMenu::load
		*/
		virtual void unload ();

		/**
		* Renders the state.
		* Always renders on 0,0 position.
		* @see Sprite::render
		*/
		virtual void render ();

		void handleSelectorMenu ();

		bool should_ignore;

		void change_shwing_animation();

		void handle_attract();

		void render_menu();

		void handle_current_selection_down_and_right( const double SELECTOR_DELAY_TIME );

		void verify_should_ignore();

		void handle_current_selection_up_and_left( const double SELECTOR_DELAY_TIME );

	private:
		enum Selection : uint8_t
		{

			NEWGAME = 0,
			CONTINUE,
			OPTIONS,
			CREDITS,
			TOTAL

		};

		Sprite *menu_image = NULL; /**< The image shown on the menu. */
		Sprite *menu_selector = NULL; /**< The selector shown on the menu. */
		Sprite *attrack_mode_background = NULL; /**< The image shown on the menu. */
		Sprite *attract_mode = NULL; /**< The selector shown on the menu. */
		double passed_time = 0.0; /**< The time already elapsed since the beggining of the menu. */

		int current_selection = 0;
		int selector_X_position_left [ Selection::TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_left [ Selection::TOTAL ]; /**< The Y position of the left selector.. */
		int selector_X_position_right [ Selection::TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_right [ Selection::TOTAL ]; /**< The Y position of the left selector.. */

		const int attract_height_size = 0; // The height size of the attract.
		const int attract_change_speed = 0; // The value to change attract speed.

		SDL_Rect attractClip; /**< The clip shown on the credits. */

		Animation *shwing_animation = NULL; // The shwing animation.
		bool is_shwing_activated = false; // Boolean variable to verify if the swing is activated.
		Sprite *shwing = NULL; // The shwing sprite.
		SDL_Rect shwing_clip = NULL; // Instante of shwing clip.
};

#endif // INCLUDE_GSTATEMENU_H
