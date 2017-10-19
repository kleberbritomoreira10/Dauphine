#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "SDLWrapper.h"
#include "Window.h"
#include "StateGame.h"

#include <map>
#include <memory>

#include "AudioHandler.h"
#include "InputHandler.h"
#include "ResourceManager.h"
#include "FadeScreen.h"
#include "GameSave.h"

/**
* Main structure class for the game.
* Contains all the necessary functionalities to loop and update the game.
* 	Is the state machine (controls current state), contains all the different possible states.
*/
class Game
{

	public:
		/**
		* All possible game states.
		*/
		enum GStates : uint8_t
		{
			SPLASH = 0,
			MENU,
			NEW_GAME,
			LEVEL_ONE,
			LEVEL_TWO,
			LEVEL_THREE,
			LEVEL_FOUR,
			LEVEL_FIVE,
			LEVEL_BOSS,
			CONTINUE,
			OPTIONS,
			CREDITS,
			GAMEOVER,
			TRANSITION,
			VICTORY
		};

		/**
		* Singleton imeplementation for Game.
		* @return The instance for a Game.
		*/
		static Game &instance ();

		/**
		* The destructor.
		* Destroys the game's Window and states, and unloads current state.
		*/
		~Game ();

		/**
		* The main game loop.
		* Orders the game structure, such as inputs, updates, and rendering.
		*/
		void runGame ();

		/**
		* Sets the current game state.
		* @see StateGame::load()
		* @see StateGame::unload()
		* @param state_ : The state you want to be changed into.
		*/
		void setState ( const GStates state_ );

		/**
		* @return The Game audio_handler.
		*/
		AudioHandler &get_audio_handler ();

		/**
		* @return The boolean array recieved from the InputHandler.
		*/
		std::array < bool, GameKeys::MAX > getInput ();

		/**
		* @return The resource manager.
		*/
		ResourceManager &getResources ();

		FadeScreen &get_fade ();

		GameSave &get_saves ();

		/**
		* Stops execution and closes the game.
		*/
		void stop ();

		void clearKeyFromInput ( const GameKeys KEY );
		void resizeWindow ( const unsigned int width_, const unsigned int height_ );

		bool is_cut_scene; // Verify if the scene is cutted.
		bool is_paused; // Verify if the scene is paused.

		unsigned int current_slot; // Current slot value.

		static const int total_number_to_be_parameterizedof_lines = 14; //Total number of lines in the slot.
		unsigned int current_line; // Current line value.

		Sprite *dialog[total_number_to_be_parameterizedof_lines];

		GStates transitionTo;

	private:
		/**
		* The constructor.
		* Sets the game window and tells the game that it is OK to begin looping. Also, it
		* 	begins the FPS manager.
		* @note If the Window cannot be created, the game will not begin.
		*/
		Game ();

		/**
		* Loads all the states.
		* Every new state implemented should be INITIALIZED here.
		*/
		void initializeStates ();

		/**
		* Deletes all the loaded states.
		* Every new state implemented should be deleted here.
		*/
		void destroyStates ();

		void renderPause ();

		bool isPauseAble ();

		void renderDialog ();

		void handleDialog ();

		void updateDialog ();

		Window *window; /**< The game Window. */
		bool is_running; /**< Whether the game is currently running/looping or not. */

		Sprite *pause_image; // Sprite to pause the image.
		Sprite *pause_selector; // Sprite to pause de selector.

		AudioHandler *audio_handler; /**< The Game AudioHandler. */
		InputHandler *input_handler; /**< The Game InputHandler. */
		ResourceManager *resource_manager; /**< The Game ResourceManager. */
		GameSave *game_save; // Save the game.
		FadeScreen *fade_screen; // Fade the screen.

		StateGame *current_state; /**< The current state, which the game is in. */

		std::map < GStates, StateGame *> states_map; /**< Map containing all possible states. */

		FPSmanager fps_manager; /**< The FPSManager from SDL2_GFX. Handles the framerate
			capping. */
		void handleSelectorMenu ();
		void updatePause ();

		enum PSelection : uint8_t
		{
			RESUME = 0,
			EXIT,
			TOTAL
		};

		double passed_time; /**< The time already elapsed since the beggining of the menu. */

		int current_selection; // The value of the current selection.
		int selector_X_position_left [ PSelection::TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_left [ PSelection::TOTAL ]; /**< The Y position of the left selector.. */
		int selector_X_position_right [ PSelection::TOTAL ]; /**< The X position of the left selector.. */
		int selector_Y_position_right [ PSelection::TOTAL ]; /**< The Y position of the left selector.. */

};

#endif //INCLUDE_GAME_H
