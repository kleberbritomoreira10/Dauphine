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
	this -> statesMap.emplace ( stateEnum, new stateClass() )
#define ADD_STATE_INSERT(stateEnum, stateClass) \
	this -> statesMap.insert ( std::make_pair <GStates, StateGame*> ( stateEnum, new stateClass() ) )
	

Game& Game::instance ()
{
	static Game* instance = new Game();
	return *instance;
}

/**
* The constructor.
* Initializes the attributes.
* @param isCutscene : Analyzes if the scene is a cutscente.
* @param isPaused : Analyzes if the game is saved.
* @param currentLine : Analyzes the state of the current line.
* @param transitionTo : Controlls the screens transitions.
* @param window : The size of the windows of the game.
* @param isRunning : Analyzes if the character is runing.
* @param pauseImage : The image used for pause the game.
* @param pauseSelector : The state when the selector is stoped in a option.
* @param audioHandler : The manager of audio for the game.
* @param inputHandler : The manager of inputs for the game.
* @param gameSave : The file of save datas for the game.
* @param currentState : The current state of the object analyzed.
* @param statesMap : The conduct of the maps.
* @param passedTime : The time passed bettwen two events.
* @param currentSelection : The current option selected in menu.
* @param selectorXPositionLeft : Selector is in left position in axis x.
* @param selectorYPositionLeft : Selector is in left position in axis y..
* @param selectorXPositionRight : Selector is in right position in axis x..
* @param selectorYPositionRight : Selector is in right position in axis y..
*/ 
Game::Game () :
	isCutscene( false ),
	isPaused( false ),
	currentLine( 0 ),
	transitionTo( LEVEL_ONE ),
	window( nullptr ),
	isRunning( false ),
	pauseImage( nullptr ),
	pauseSelector( nullptr ),
	audioHandler( new AudioHandler() ),
	inputHandler( new InputHandler() ),
	resourceManager( new ResourceManager() ),
	gameSave( new GameSave() ),
	fadeScreen( nullptr ),
	currentState( nullptr ),
	statesMap(),
	passedTime( 0.0 ),
	currentSelection( PSelection::RESUME ),
	selectorXPositionLeft { 550, 550 },
	selectorYPositionLeft { 400, 470 },
	selectorXPositionRight { 930, 930 },
	selectorYPositionRight { 400, 470 }
{
	this->window = new Window( Configuration::getScreenWidth(),
		Configuration::getScreenHeight(), Configuration::getWindowTitle() );

	assert( this->window != nullptr && "The window should not be null!" );

	initializeStates();

	std::string path = "res/images/Dialog/dialog";
	std::string extension = ".png";	

	for( int i = 0; i < numLines; i++ )
	{
		this->dialog[ i ] = nullptr;
		this->dialog[ i ] = getResources().get( path + Util::toString(i) + extension );
	
		if( this->dialog[i] == nullptr )
		{
			Log( ERROR ) << "Invalid dialog image.";
		}
	}

	this->pauseImage = getResources().get("res/images/pause_overlay.png");
	this->pauseSelector = getResources().get("res/images/cursor_regular.png");
	this->pauseSelector -> setWidth(50);

	this->isRunning = true;
	FPSWrapper::initialize( this -> fpsManager );
}

Game::~Game()
{
	if( this -> currentState != nullptr )
	{
		this -> currentState -> unload();
	}

	destroyStates();

	if( this -> audioHandler != nullptr )
	{
		delete this -> audioHandler;
	}
	if( this -> inputHandler != nullptr )
	{
		delete this->inputHandler;
	}
	if( this -> resourceManager != nullptr )
	{
		delete this -> resourceManager;
	}
	if( this -> fadeScreen != nullptr )
	{
		delete this -> fadeScreen;
	}

	if( this -> window != nullptr )
	{
		delete this -> window;
		this -> window = nullptr;
	}
}

void Game::runGame()
{
	this -> fadeScreen = new FadeScreen();

	this -> currentState = this -> statesMap.at(GStates::SPLASH);
	this -> currentState -> load();

	// Get the first game time.
	double totalGameTime = 0.0;
	const double deltaTime = 1.0 / 60.0;
	double accumulatedTime = 0.0;

	// This is the main game loop.
	while( this -> isRunning )
	{

		const double frameTime = FPSWrapper::delay( this -> fpsManager );
		accumulatedTime += frameTime;

		// Update.
		while( accumulatedTime >= deltaTime )
		{
			this -> inputHandler -> handleInput();

			// Check for an exit signal from input.
			if( this -> inputHandler -> isQuitFlag() == true )
			{
				stop();
				return;
			}

			std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

			if( keyStates[ GameKeys::ESCAPE ] && isPauseAble() )
			{
				this -> currentSelection = PSelection::RESUME;
				this -> isPaused = true;
			}

			if( !this -> isPaused )
			{
				this -> currentState -> update( deltaTime );
			}
			else if( !this -> isCutscene)
			{
				this -> passedTime += deltaTime;
				updatePause();
			}
			else
			{
				this -> passedTime += deltaTime;
				updateDialog();
			}

			this -> fadeScreen -> update( deltaTime );

			accumulatedTime -= deltaTime;
			totalGameTime += deltaTime;
		}

		// Render of window.
		window -> clear();
		
		this -> currentState -> render();				    
		if( this -> isPaused )
		{
			renderPause();
		}
		else if( this -> isCutscene )
		{
			if( currentLine < numLines )
				renderDialog();
			else
			{
				currentLine = 0;
				isCutscene = false;
			}
		}

		this -> fadeScreen -> render();

		window -> render();
		
	}

}

void Game::setState( const GStates state_ )
{
	/// @todo Implement the transition between states.
	this -> currentState -> unload();
	this -> currentState = this -> statesMap.at( state_ );
	this -> currentState -> load();
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

	if( currentLine > numLines )
	{
		currentLine = 0;
		return;
	}

	if( this -> dialog[ currentLine ] )
		this -> dialog[ currentLine ] -> render( 0, 0, nullptr, true );
		
}

void Game::handleDialog()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	const double selectorDelayTime = 0.2;

	if( keyStates[ GameKeys::SPACE ] == true )
	{
		if( this -> passedTime >= selectorDelayTime )
		{
			currentLine++;
		}
	}
}

void Game::updateDialog()
{

	handleDialog();

}

void Game::renderPause()
{
	if( this -> pauseImage != nullptr )
	{
		this -> pauseImage -> render( 0, 0, nullptr, true );

		this -> pauseSelector -> render( selectorXPositionLeft[ currentSelection ],
			selectorYPositionLeft[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

		this -> pauseSelector -> render( selectorXPositionRight[ currentSelection ],
			selectorYPositionRight[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );
	}
	else
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

	const double selectorDelayTime = 0.2;

	if( keyStates[ GameKeys::DOWN ] == true || keyStates[ GameKeys::RIGHT ] == true )
	{
		if( this -> passedTime >= selectorDelayTime )
		{
			if( currentSelection < ( PSelection::TOTAL - 1 )){
				currentSelection++;
			}
			else
			{
				currentSelection = PSelection::RESUME;
			}
			this -> passedTime = 0.0;
		}
	}
	else if( keyStates[ GameKeys::UP ] == true || keyStates[ GameKeys::LEFT ] == true )
	{
		if(this -> passedTime >= selectorDelayTime )
		{
			if( currentSelection > PSelection::RESUME )
			{
				currentSelection--;
			}
			else
			{
				currentSelection = ( PSelection::TOTAL - 1 );
			}
			this -> passedTime = 0.0;
		}
	}
	else if( currentSelection == PSelection::RESUME && keyStates[ GameKeys::SPACE ] == true )
	{
		this -> isPaused = false;
	}

	else if( currentSelection == PSelection::EXIT && keyStates[ GameKeys::SPACE ] == true)
	{
		Game::instance().setState( Game::GStates::MENU );
		this -> isPaused = false;
	}
}

void Game::destroyStates()
{
	std::map<GStates, StateGame*>::const_iterator iterator;
    for( iterator = this -> statesMap.begin(); iterator != this -> statesMap.end(); iterator++ )
    {
        delete iterator -> second;
    }
}

AudioHandler& Game::getAudioHandler()
{
	return ( *( this->audioHandler ));
}

std::array<bool, GameKeys::MAX> Game::getInput()
{
	return this -> inputHandler -> getKeyStates();
}

ResourceManager& Game::getResources()
{
	return ( *( this -> resourceManager ) );
}

GameSave& Game::getSaves()
{
	return ( *( this -> gameSave ) );
}

void Game::stop()
{
	this -> isRunning = false;
}

void Game::clearKeyFromInput( const GameKeys key_ )
{
	this -> inputHandler -> clearKey( key_ );
}

FadeScreen& Game::getFade()
{
	return ( *( this -> fadeScreen ));
}

void Game::resizeWindow( const unsigned int width_, const unsigned int height_ )
{
	this -> window -> resize( width_, height_ );
}

bool Game::isPauseAble()
{

	if( this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_ONE ) )
	{
		return true;
	}
	if( this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_TWO ) )
	{
		return true;
	}
	if( this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_THREE ) )
	{
		return true;
	}
	if( this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_FOUR ) )
	{
		return true;
	}
	if(this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_FIVE ) )
	{
		return true;
	}
	if(this -> currentState == this -> statesMap.at( Game::GStates::LEVEL_BOSS ) )
	{
		return true;
	}

	return false;
}
