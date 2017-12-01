#ifndef INCLUDE_GSTATEOPTIONS_H
#define INCLUDE_GSTATEOPTIONS_H

#include "StateGame.h"
#include "Sprite.h"
#include "Text.h"

#define MAX_NUMBER_OF_RESOLUTIONS 3

/**
* The state for the initial menu screen.
* Game state that will contain the game over screen.
*/
class GStateOptions : public StateGame
{

	public:
		/**
		* The constructor.
		* Initializes all the attributes.
		*/
		GStateOptions ();

		/**
		* The destructor.
		*/
		virtual ~GStateOptions ();

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
		* @see GStateOptions::load
		*/
		virtual void unload ();

		/**
		* Renders the state.
		* Always renders on 0,0 position.
		* @see Sprite::render
		*/
		virtual void render ();

		void delete_resolution();

		void delete_music_volume_text();

		void delete_sfx_volume_text();

		void handle_current_option_down_key( const double SELECTOR_DELAY_TIME );

		void handle_current_option_up_key( const double SELECTOR_DELAY_TIME );

		void handle_current_option_left_key( const double SELECTOR_DELAY_TIME );

		void handle_current_option_right_key( const double SELECTOR_DELAY_TIME );

		void render_options_image();

		void render_selector();

		void apply_volume();

	private:
		void applyOptions ();

		double elapsedTime = 0.0;

		enum Option : uint8_t
		{
			OPTIONS_RESOLUTION = 0,
			OPTIONS_VOLUME_MUSIC,
			OPTIONS_VOLUME_SFX,
			OPTIONS_APPLY,
			OPTIONS_RETURN,
			OPTIONS_TOTAL
		};

		enum Resolution : uint8_t
		{
			RESOLUTION_800_600 = 0,
			RESOLUTION_768_432,
			RESOLUTION_960_540,
			RESOLUTION_TOTAL
		};

		Sprite *options_image; /**< The image shown on the menu. */

		uint8_t current_resolution;
		uint8_t current_option;

		Sprite *selector; /**< The selector shown on the menu. */

		int selector_X_position_left [ Option::OPTIONS_TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_left [ Option::OPTIONS_TOTAL ]; /**< The Y position of the left selector.. */
		int selector_X_position_right [ Option::OPTIONS_TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_right [ Option::OPTIONS_TOTAL ]; /**< The Y position of the left selector.. */

		static const std::string possible_resolutions [ MAX_NUMBER_OF_RESOLUTIONS ]; // Array of possible resolutions >= 3

		unsigned int music_volume; // The value of the music volume.
		unsigned int sfx_volume; // The value of the sfx volume.

		Text *resolution;
		Text *music_volume_text; // The text  of music volume.
		Text *sfx_volume_text; // The text of sfx volume.
};

#endif // INCLUDE_GSTATEOPTIONS_H
