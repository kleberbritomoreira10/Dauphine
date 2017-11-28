#include "Game.h"
#include "FPSWrapper.h"
#include "Configuration.h"
#include <cassert>
#include "Util.h"

#include "GStateSplash.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "LevelThree.h"
#include "LevelFour.h"
#include "LevelFive.h"
#include "LevelBoss.h"
#include "GStateMenu.h"
#include "GStateNewGame.h"
#include "GStateContinue.h"
#include "GStateOptions.h"
#include "GStateCredits.h"
#include "GStateGameOver.h"
#include "GStateTransition.h"
#include "GStateVictory.h"
#include "Sprite.h"

#include "Logger.h"

#define ADD_STATE_EMPLACE ( stateEnum, stateClass ) \
	this -> states_map.emplace ( stateEnum, new stateClass() )
#define ADD_STATE_INSERT( stateEnum, stateClass ) \
	this -> states_map.insert ( std::make_pair < GStates, StateGame * > ( stateEnum, new stateClass() ) )

#define INITIAL_TIME 0
#define POSITION_X 0
#define POSITION_Y 0
#define WIDTH_VALUE 50
#define NO_NUMBERS_TO_BE_PARAMETERIZED 0
#define NO_LINES 0

Game &Game::instance ()
{
	static Game *instance = new Game();
	return *instance;
}

//Initializes method constructor with fundamental elements for playing the game.
Game::Game () :
	is_cut_scene( false ),
	is_paused( false ),
	current_line( 0 ),
	transitionTo( LEVEL_ONE ),
	window( nullptr ),
	is_running( false ),
	pause_image( nullptr ),
	pause_selector( nullptr ),
	audio_handler( new AudioHandler() ),
	input_handler( new InputHandler() ),
	resource_manager( new ResourceManager() ),
	game_save( new GameSave() ),
	fade_screen( nullptr ),
	current_state( nullptr ),
	states_map(),
	passed_time( 0.0 ),
	current_selection( PSelection::RESUME ),
	selector_X_position_left { 550, 550 },
	selector_Y_position_left { 400, 470 },
	selector_X_position_right { 930, 930 },
	selector_Y_position_right { 400, 470 }
{

	Log( INFO ) << "Initializing constructor method";

	this -> window = new Window( Configuration::getScreenWidth(),
		Configuration::getScreenHeight(), Configuration::getWindowTitle() ); // Create a new window instance.

	assert( this -> window != nullptr && "The window should not be null!" ); // Check if the menu is not null.

	initializeStates();

	getDialog();


	this -> pause_image = getResources().get("res/images/pause_overlay.png"); // Get the paused menu image.
	this -> pause_selector = getResources().get("res/images/cursor_regular.png"); // Get the cursor image.
	this -> pause_selector -> setWidth( WIDTH_VALUE ); // Set the pause selector width.

	this -> is_running = true;
	FPSWrapper::initialize( this -> fps_manager );
}

// Get all the dialogs image.
void Game::getDialog()
{
	std::string path = "res/images/Dialog/dialog"; // The dialog image path.
	std::string extension = ".png";

	for( int i = 0; i < total_number_to_be_parameterized; i++ )
	{
		 this -> dialog[ i ] = nullptr;
		 this -> dialog[ i ] = getResources().get( path + Util::toString(i) + extension ); // Get the dialog image.

		if( this -> dialog[i] == nullptr )
		{
			Log( ERROR ) << "Invalid dialog image.";

		}else
		{
			Log( DEBUG ) << "Valid dialog image.";
		}
	}
}

Game::~Game()
{
	unload_current_state();

	destroyStates();

	delete_audio_handler();

	delete_input_handler();

	delete_resource_manager();

	delete_fade_screen();

	delete_window();
}

void Game::unload_current_state()
{
	if( this -> current_state != nullptr )
	{
		this -> current_state -> unload(); // Unload the current state.

	}else
	{
		Log( DEBUG ) << "The current state is already null";
	}
}

void Game::delete_audio_handler()
{
	if( this -> audio_handler != nullptr )
	{
		delete this -> audio_handler; // Delete the audio handler.

	}else
	{
		Log( DEBUG ) << "The audio handler is already null";
	}
}

void Game::delete_input_handler()
{
	if( this -> input_handler != nullptr )
	{
		delete this -> input_handler; // Delete the input handler.

	}else
	{
		Log( DEBUG ) << "The input handler is already null";
	}
}

void Game::delete_resource_manager()
{
	if( this -> resource_manager != nullptr )
	{
		delete this -> resource_manager; // Delete the resource manager.

	}else
	{
		Log( DEBUG ) << "The resource manager is already null";
	}

}

void Game::delete_fade_screen()
{
	if( this -> fade_screen != nullptr )
	{
		delete this -> fade_screen; // Delete the fade screen.

	}else
	{
		Log( DEBUG ) << "The fade screen is already null";
	}
}

void Game::delete_window()
{
	if( this -> window != nullptr )
	{
		delete this -> window; // Delete the window.
		this -> window = nullptr;

	}else
	{
		Log( DEBUG ) << "The window is already null";
	}
}

void Game::runGame()
{
	this -> fade_screen = new FadeScreen(); // Create a new instance of fade screen.

	this -> current_state = this -> states_map.at( GStates::SPLASH ); // Get the state splash.
	this -> current_state -> load();

	// Get the first game time.
	double total_game_time = INITIAL_TIME;
	const double delta_time = 1.0 / 60.0;
	double accumulated_time = INITIAL_TIME;

	// This is the main game loop.
	while( this -> is_running )
	{

		const double frame_time = FPSWrapper::delay( this -> fps_manager ); // Get the fps manager.

		accumulated_time += frame_time; // Increment the acumulated time.

		// Update.
		while( accumulated_time >= delta_time )
		{
			this -> input_handler -> handleInput(); // Calls the method to handle the inputs.

			check_exit_signal();

			get_current_selection();

			assert( delta_time > INITIAL_TIME ); // Check if variates the time.

			time_verifications( delta_time );

			this -> fade_screen -> update( delta_time ); // Updates the fade screen.

			accumulated_time -= delta_time;
			total_game_time += delta_time;
		}

		window -> clear(); // Render of window.

		this -> current_state -> render(); // Render current state

		assert( total_number_to_be_parameterized > NO_NUMBERS_TO_BE_PARAMETERIZED ); // Check if the there is any number
																					// to be parameterized.

		render_pause_and_dialog();

		this -> fade_screen -> render(); // Render fade screen.

		window -> render(); // Render window.

	}

	Log( INFO ) << "Game rendered.";

}

// Check for an exit signal from input.
void Game::check_exit_signal()
{
	if( this -> input_handler -> isQuitFlag() == true )
	{
		stop();
		return;

	}else
	{
		Log( DEBUG ) << "Quit flag is false.";
	}
}

void Game::get_current_selection()
{
	std::array < bool, GameKeys::MAX > keyStates = Game::instance().getInput(); // Get inputs.

	if( keyStates[ GameKeys::ESCAPE ] && isPauseAble() )
	{
		this -> current_selection = PSelection::RESUME;
		this -> is_paused = true;

	}else
	{
		Log( DEBUG ) << "State is not pausable.";
	}
}

/**
* Make time verifications.
* @param delta_time: Time variation.
*/
void Game::time_verifications(const double delta_time)
{
	if( !this -> is_paused )
	{

		this -> current_state -> update( delta_time ); // Update the current state.

	}
	else if( !this -> is_cut_scene)
	{
		this -> passed_time += delta_time; // Increment passed time.
		updatePause();
	}
	else
	{
		this -> passed_time += delta_time; // Increment passed time.
		updateDialog();
	}

}

/**
* Renders the pause and dialog screen.
*/
void Game::render_pause_and_dialog()
{
	if( this -> is_paused )
	{
		renderPause();
	}
	else if( this -> is_cut_scene )
	{
		if( current_line < total_number_to_be_parameterized )
			renderDialog();
		else
		{
			current_line = NO_LINES;
			is_cut_scene = false;
		}
	}else
	{
		Log( DEBUG ) << "is_cut_scene is false.";
	}
}

/**
* Sets all the states.
* @param state_: The state to be setted.
*/
void Game::setState( const GStates state_ )
{
	/// @todo Implement the transition between states.
	this -> current_state -> unload(); // Unload current state.
	this -> current_state = this -> states_map.at( state_ ); // Get the current state.
	this -> current_state -> load(); // Load the current state.
}

/**
* Loads all the states.
* Every new state implemented should be INITIALIZED here.
*/
void Game::initializeStates()
{
	// Initialize all the states in Game here.

	// Emplace the states pointers onto the map.
	ADD_STATE_INSERT( SPLASH, GStateSplash );
	ADD_STATE_INSERT( MENU, GStateMenu );
	ADD_STATE_INSERT( NEW_GAME, GStateNewGame );
	ADD_STATE_INSERT( LEVEL_ONE, LevelOne );
	ADD_STATE_INSERT( LEVEL_TWO, LevelTwo );
	ADD_STATE_INSERT( LEVEL_THREE, LevelThree );
	ADD_STATE_INSERT( LEVEL_FOUR, LevelFour );
	ADD_STATE_INSERT( LEVEL_FIVE, LevelFive );
	ADD_STATE_INSERT( LEVEL_BOSS, LevelBoss );
	ADD_STATE_INSERT( CONTINUE, GStateContinue );
	ADD_STATE_INSERT( OPTIONS, GStateOptions );
	ADD_STATE_INSERT( CREDITS, GStateCredits );
	ADD_STATE_INSERT( GAMEOVER, GStateGameOver );
	ADD_STATE_INSERT( TRANSITION, GStateTransition );
	ADD_STATE_INSERT( VICTORY, GStateVictory );
}

/**
* Render the dialog screen.
*/
void Game::renderDialog()
{

	verify_current_line();

	if( this -> dialog[ current_line ] )
	{
		this -> dialog[ current_line ] -> render( POSITION_X, POSITION_Y, nullptr, true ); // Render dialog.

	}else
	{
		Log( DEBUG ) << "Dialog is null.";
	}

}

/**
* Verify the current line.
*/
void Game::verify_current_line()
{
	if( current_line > total_number_to_be_parameterized )
	{
		current_line = NO_LINES;
		return;
	}else
	{
		Log( DEBUG ) << "current_line < total_number_to_be_parameterized";
	}
}

/**
* Handle the dialogs.
*/
void Game::handleDialog()
{
	std::array < bool, GameKeys::MAX > keyStates = Game::instance().getInput(); // Get inputs.

	const double SELECTOR_DELAY_TIME = 0.2; 

	if( keyStates[ GameKeys::SPACE ] == true )
	{
		if( this -> passed_time >= SELECTOR_DELAY_TIME )
		{
			current_line++;
		}else
		{
			Log( DEBUG ) << "passed_time <= SELECTOR_DELAY_TIME";
		}
	}else
	{
		Log( DEBUG ) << "Key SPACE is false";
	}
}

void Game::updateDialog()
{

	handleDialog();

}

/**
* Render the pause screen.
*/
void Game::renderPause()
{
	if( this -> pause_image != nullptr )
	{
		this -> pause_image -> render( POSITION_X, POSITION_Y, nullptr, true ); // Render pause image.

		// Render pause selector.
		this -> pause_selector -> render( selector_X_position_left[ current_selection ],
			selector_Y_position_left[ current_selection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE ); 

		// Render pause selector horizontal.
		this -> pause_selector -> render( selector_X_position_right[ current_selection ],
			selector_Y_position_right[ current_selection ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL ); 

	}else
	{
		Log( WARN ) << "No image set to display on the menu!";
	}
}

void Game::updatePause()
{

	handleSelectorMenu();
}

/**
* Handle the selector menu.
*/
void Game::handleSelectorMenu()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput(); // Get inputs.

	const double SELECTOR_DELAY_TIME = 0.2;

	if( keyStates[ GameKeys::DOWN ] == true || keyStates[ GameKeys::RIGHT ] == true )
	{

		handle_selection_keys_down_and_right( SELECTOR_DELAY_TIME ); 

	}else if( keyStates[ GameKeys::UP ] == true || keyStates[ GameKeys::LEFT ] == true )
	{
		
		handle_selection_keys_up_and_left( SELECTOR_DELAY_TIME );

	}else if( current_selection == PSelection::RESUME && keyStates[ GameKeys::SPACE ] == true )
	{
		this -> is_paused = false;

	}else if( current_selection == PSelection::EXIT && keyStates[ GameKeys::SPACE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );
		this -> is_paused = false;

	}else
	{
		Log( DEBUG ) << "Key SPACE is null";
	}
}

/**
* Handle the selection when key DOWN and RIGHT is selected.
* @param SELECTOR_DELAY_TIME: The delay of the selector.
*/
void Game::handle_selection_keys_down_and_right( const double SELECTOR_DELAY_TIME )
{
	if( this -> passed_time >= SELECTOR_DELAY_TIME )
	{
		if( current_selection < ( PSelection::TOTAL - 1 ))
		{
			current_selection++;

		}else
		{
			current_selection = PSelection::RESUME;
		}

		this -> passed_time = INITIAL_TIME; // Set passed time to 0.

	}else
	{
		Log( DEBUG ) << "passed_time <= SELECTOR_DELAY_TIME";
	}
}

/**
* Handle the selection when key UP and LEFT is selected.
* @param SELECTOR_DELAY_TIME: The delay of the selector.
*/
void Game::handle_selection_keys_up_and_left( const double SELECTOR_DELAY_TIME )
{
	if( this -> passed_time >= SELECTOR_DELAY_TIME )
	{
		if( current_selection > PSelection::RESUME )
		{
			current_selection--;

		}else
		{
			current_selection = ( PSelection::TOTAL - 1 );
		}

		this -> passed_time = INITIAL_TIME; // Setting the passed time to 0.

	}else
	{
		Log( DEBUG ) << "passed_time <= SELECTOR_DELAY_TIME";
	}
}

/**
* Destroy all the states.
*/
void Game::destroyStates()
{
	Log( INFO ) << "Destroying the states.";

	std::map < GStates, StateGame * >::const_iterator iterator; // Create an iterator.

    for( iterator = this -> states_map.begin(); iterator != this -> states_map.end(); iterator++ )
    {
        delete iterator -> second;
    }
}

/**
* Get the audio handler.
* @return The Game audio_handler.
*/
AudioHandler &Game::get_audio_handler()
{
	Log( INFO ) << "Getting audio handler.";
	return ( *( this -> audio_handler )); 
}

/**
 * Get the user input.
* @return The boolean array recieved from the InputHandler.
*/
std::array < bool, GameKeys::MAX > Game::getInput()
{
	Log( INFO ) << "Getting inputs.";
	return this -> input_handler -> getKeyStates();
}

/**
 * Get all the resources.
* @return The resource manager.
*/
ResourceManager &Game::getResources()
{
	Log( INFO ) << "Getting the resource manager.";
	return ( *( this -> resource_manager ) );
}

/**
 * Get all the game saves.
* @return The game save.
*/
GameSave &Game::get_saves()
{
	Log( INFO ) << "Getting game saves.";
	return ( *( this -> game_save ) );
}

/**
* Stops execution and closes the game.
*/
void Game::stop()
{
	this -> is_running = false;
}

/**
* Clear the keys from the input.
* @param KEY: keys from the input.
*/
void Game::clearKeyFromInput( const GameKeys KEY )
{
	this -> input_handler -> clearKey( KEY );
}

/**
 * Get the fade.
* @return The fade screen.
*/
FadeScreen &Game::get_fade()
{
	Log( INFO ) << "Getting fade screen.";
	return ( *( this -> fade_screen ));
}

/**
* Stops execution and closes the game.
* @param width_: The new width value of the input.
* @param height_: The new height value of the window.
*/
void Game::resizeWindow( const unsigned int width_, const unsigned int height_ )
{
	Log( INFO ) << "Resizing the window.";
	this -> window -> resize( width_, height_ ); // Resizes the window
}

/**
* Verify if the game can be paused in an specific moment.
* @return: Boolean value. True: The game can be paused. False: The game cannot be paused.
*/
bool Game::isPauseAble()
{

	if( this -> current_state == this -> states_map.at( Game::GStates::LEVEL_ONE ) )
	{
		return true;

	}else
	{
		return false;
	}

	if( this -> current_state == this -> states_map.at( Game::GStates::LEVEL_TWO ) )
	{
		return true;

	}else
	{
		return false;
	}

	if( this -> current_state == this -> states_map.at( Game::GStates::LEVEL_THREE ) )
	{
		return true;

	}else
	{
		return false;
	}

	if( this -> current_state == this -> states_map.at( Game::GStates::LEVEL_FOUR ) )
	{
		return true;

	}else
	{
		return false;
	}

	if(this -> current_state == this -> states_map.at( Game::GStates::LEVEL_FIVE ) )
	{
		return true;

	}else
	{
		return false;
	}

	if(this -> current_state == this -> states_map.at( Game::GStates::LEVEL_BOSS ) )
	{
		return true;

	}else
	{
		return false;
	}
}
