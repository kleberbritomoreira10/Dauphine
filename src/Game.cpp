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
	this -> window = new Window( Configuration::getScreenWidth(),
		Configuration::getScreenHeight(), Configuration::getWindowTitle() );

	assert( this -> window != nullptr && "The window should not be null!" );

	initializeStates();

	getDialog();


	this -> pause_image = getResources().get("res/images/pause_overlay.png");
	this -> pause_selector = getResources().get("res/images/cursor_regular.png");
	this -> pause_selector -> setWidth( WIDTH_VALUE );

	this -> is_running = true;
	FPSWrapper::initialize( this -> fps_manager );
}

// Get all the dialogs image.
void Game::getDialog()
{
	std::string path = "res/images/Dialog/dialog";
	std::string extension = ".png";

	for( int i = 0; i < total_number_to_be_parameterized; i++ )
	{
		 this -> dialog[ i ] = nullptr;
		 this -> dialog[ i ] = getResources().get( path + Util::toString(i) + extension );

		if( this -> dialog[i] == nullptr )
		{
			Log( ERROR ) << "Invalid dialog image.";

		}else
		{
			// No action.
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
		this -> current_state -> unload();

	}else
	{
		// No action.
	}
}

void Game::delete_audio_handler()
{
	if( this -> audio_handler != nullptr )
	{
		delete this -> audio_handler;

	}else{
			// No action.
	}
}

void Game::delete_input_handler()
{
	if( this -> input_handler != nullptr )
	{
		delete this -> input_handler;

	}else
	{
		// No action.
	}
}

void Game::delete_resource_manager()
{
	if( this -> resource_manager != nullptr )
	{
		delete this -> resource_manager;

	}else
	{
		// No action.
	}

}

void Game::delete_fade_screen()
{
	if( this -> fade_screen != nullptr )
	{
		delete this -> fade_screen;

	}else
	{
		// No action.
	}
}

void Game::delete_window()
{
	if( this -> window != nullptr )
	{
		delete this -> window;
		this -> window = nullptr;

	}else
	{
		// No action.
	}
}

void Game::runGame()
{
	this -> fade_screen = new FadeScreen();

	this -> current_state = this -> states_map.at( GStates::SPLASH );
	this -> current_state -> load();

	// Get the first game time.
	double total_game_time = INITIAL_TIME;
	const double delta_time = 1.0 / 60.0;
	double accumulated_time = INITIAL_TIME;

	// This is the main game loop.
	while( this -> is_running )
	{

		const double frame_time = FPSWrapper::delay( this -> fps_manager );
		accumulated_time += frame_time;

		// Update.
		while( accumulated_time >= delta_time )
		{
			this -> input_handler -> handleInput();

			check_exit_signal();

			get_current_selection();

			assert( delta_time > INITIAL_TIME );

			time_verifications(delta_time);

			this -> fade_screen -> update( delta_time );

			accumulated_time -= delta_time;
			total_game_time += delta_time;
		}

		// Render of window.
		window -> clear();

		this -> current_state -> render();

		assert( total_number_to_be_parameterized > NO_NUMBERS_TO_BE_PARAMETERIZED );

		render_pause_and_dialog();

		this -> fade_screen -> render();

		window -> render();

	}

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
		// No action.
	}
}

void Game::get_current_selection()
{
	std::array < bool, GameKeys::MAX > keyStates = Game::instance().getInput();

	if( keyStates[ GameKeys::ESCAPE ] && isPauseAble() )
	{
		this -> current_selection = PSelection::RESUME;
		this -> is_paused = true;

	}else
	{
		// No action.
	}
}

void Game::time_verifications(const double delta_time)
{
	if( !this -> is_paused )
	{

		this -> current_state -> update( delta_time );

	}
	else if( !this -> is_cut_scene)
	{
		this -> passed_time += delta_time;
		updatePause();
	}
	else
	{
		this -> passed_time += delta_time;
		updateDialog();
	}

}

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
		//No action.
	}
}

void Game::setState( const GStates state_ )
{
	/// @todo Implement the transition between states.
	this -> current_state -> unload();
	this -> current_state = this -> states_map.at( state_ );
	this -> current_state -> load();
}

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

void Game::renderDialog()
{

	verify_current_line();

	if( this -> dialog[ current_line ] )
	{
		this -> dialog[ current_line ] -> render( POSITION_X, POSITION_Y, nullptr, true );

	}else
	{
		// No action.
	}

}

void Game::verify_current_line()
{
	if( current_line > total_number_to_be_parameterized )
	{
		current_line = NO_LINES;
		return;
	}else
	{
		// No action.
	}
}

void Game::handleDialog()
{
	std::array < bool, GameKeys::MAX > keyStates = Game::instance().getInput();

	const double SELECTOR_DELAY_TIME = 0.2;

	if( keyStates[ GameKeys::SPACE ] == true )
	{
		if( this -> passed_time >= SELECTOR_DELAY_TIME )
		{
			current_line++;
		}else
		{
			// No action.
		}
	}else
	{
		// No action.
	}
}

void Game::updateDialog()
{

	handleDialog();

}

void Game::renderPause()
{
	if( this -> pause_image != nullptr )
	{
		this -> pause_image -> render( POSITION_X, POSITION_Y, nullptr, true );

		this -> pause_selector -> render( selector_X_position_left[ current_selection ],
			selector_Y_position_left[ current_selection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

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

void Game::handleSelectorMenu()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

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
		// No action.
	}
}

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
		this -> passed_time = INITIAL_TIME;

	}else
	{
		// No action.
	}
}

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

		this -> passed_time = INITIAL_TIME;

	}else
	{
		// No action.
	}
}

void Game::destroyStates()
{
	std::map < GStates, StateGame * >::const_iterator iterator;

    for( iterator = this -> states_map.begin(); iterator != this -> states_map.end(); iterator++ )
    {
        delete iterator -> second;
    }
}

AudioHandler &Game::get_audio_handler()
{
	return ( *( this -> audio_handler ));
}

std::array < bool, GameKeys::MAX > Game::getInput()
{
	return this -> input_handler -> getKeyStates();
}

ResourceManager &Game::getResources()
{
	return ( *( this -> resource_manager ) );
}

GameSave &Game::get_saves()
{
	return ( *( this -> game_save ) );
}

void Game::stop()
{
	this -> is_running = false;
}

void Game::clearKeyFromInput( const GameKeys KEY )
{
	this -> input_handler -> clearKey( KEY );
}

FadeScreen &Game::get_fade()
{
	return ( *( this -> fade_screen ));
}

void Game::resizeWindow( const unsigned int width_, const unsigned int height_ )
{
	this -> window -> resize( width_, height_ );
}

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
