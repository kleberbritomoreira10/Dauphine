/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @InputHandler.cpp
* Handles player input. Using SDL event handling, recieves the player input accordingly.
*/

#include "InputHandler.h"
#include "Logger.h"

/**
* The constructor.
* Used to create the input handler instance.
*/
InputHandler::InputHandler() :
	controllerHandler ( new ControllerHandler() ),
	quitFlag ( false )
{
	this -> keyStates.fill ( false );
}

/**
* The destructor.
* Deletes the InputHandler instance.
*/
InputHandler::~InputHandler()
{
	if ( this -> controllerHandler != nullptr )
	{
		delete this -> controllerHandler;
		this -> controllerHandler = nullptr;
	}	
}

/**
* Handles the input.
* Detects the pending events, and handles them appropriately.
*/
void InputHandler::handleInput ()
{
	this -> keyStates[ GameKeys::SPACE ] = false;
	this -> keyStates[ GameKeys::ROLL ] = false;
	this -> keyStates[ GameKeys::LATTACK ] = false;
	this -> keyStates[ GameKeys::NLATTACK ] = false;
	this -> keyStates[ GameKeys::ACTION ] = false;


	int pendingEvent = 0;

	do
	{

		pendingEvent = SDL_PollEvent ( &this -> sdlEvent ); 

		if (this -> sdlEvent.type == SDL_CONTROLLERBUTTONDOWN 
			|| this -> sdlEvent.type == SDL_CONTROLLERBUTTONUP 
			|| this -> sdlEvent.type == SDL_CONTROLLERAXISMOTION )
		{
			
			this -> controllerHandler -> handleInput (this -> sdlEvent );

			for( unsigned int i = 0; i < this -> keyStates.size(); i++ )
			{
				this -> keyStates[ i ] = this -> controllerHandler -> keyStates[ i ];
			}
			
		}
		// On keydown.
		
		if ( this -> sdlEvent.type == SDL_KEYDOWN )
		{
			switch ( this -> sdlEvent.key.keysym.sym )
			{
				case SDLK_SPACE: // Jump.
					if ( this -> sdlEvent.key.repeat == 0 )
					{
						this -> keyStates[ GameKeys::SPACE ] = true;
					}	
					break;

				case SDLK_UP: // UP.
					this -> keyStates[ GameKeys::UP ] = true;
					break;

				case SDLK_DOWN: // DOWN.
					this -> keyStates[ GameKeys::DOWN ] = true;
					break;

				case SDLK_LEFT: // Move left.
					this -> keyStates[ GameKeys::LEFT ] = true;
					break;

				case SDLK_RIGHT: // Move right.
					this -> keyStates[ GameKeys::RIGHT ] = true;
					break;

				case SDLK_c: // roll.
					if(this -> sdlEvent.key.repeat == 0)
					{
						this -> keyStates[ GameKeys::ROLL ] = true;
					}
					break;

				case SDLK_LCTRL: // crouch
					this -> keyStates[ GameKeys::CROUCH ] = true;
					break;

				case SDLK_a: // a.
					if(this -> sdlEvent.key.repeat == 0)
					{
						this -> keyStates[ GameKeys::ACTION ] = true;
					}
					break;

				case SDLK_LSHIFT: // d.
					this -> keyStates[ GameKeys::AIM ] = true;
					break;

				case SDLK_z: // z.
					if ( this -> sdlEvent.key.repeat == 0 )
					{
						this -> keyStates[ GameKeys::LATTACK ] = true;
					}
					break;

				case SDLK_TAB:
					this -> keyStates[ GameKeys::ITEMS ] = true;
					break;

				case SDLK_ESCAPE: // Esc.
					if ( this -> sdlEvent.key.repeat == 0 )
					{
						this -> keyStates[ GameKeys::ESCAPE ] = true;
					}
					break;

				default:
					break;
			}
		}

		// On keyup.
		else if ( this -> sdlEvent.type == SDL_KEYUP )
		{	
			switch ( this -> sdlEvent.key.keysym.sym )
			{
				case SDLK_SPACE: // Jump.
					this -> keyStates[ GameKeys::SPACE ] = false;
					break;
			
				case SDLK_UP: // UP.
					this -> keyStates[ GameKeys::UP ] = false;
					break;
			
				case SDLK_DOWN: // DOWN.
					this -> keyStates[ GameKeys::DOWN ] = false;
					break;
			
				case SDLK_LEFT: // Move left.
					this -> keyStates[ GameKeys::LEFT ] = false;
					break;
			
				case SDLK_RIGHT: // Move right.
					this -> keyStates[ GameKeys::RIGHT ] = false;
					break;

				case SDLK_c: // Roll.
					this -> keyStates[ GameKeys::ROLL ] = false;
					break;

				case SDLK_LCTRL: // crouch
					this -> keyStates[ GameKeys::CROUCH ] = false;

				case SDLK_a: // a.
					this -> keyStates[ GameKeys::ACTION ] = false;
					break;

				case SDLK_LSHIFT: // d.
					this -> keyStates[ GameKeys::AIM ] = false;
					break;

				case SDLK_z: // z.
					this -> keyStates[ GameKeys::LATTACK ] = false;
					break;

				case SDLK_TAB:
					this -> keyStates[ GameKeys::ITEMS ] = false;
					break;

				case SDLK_ESCAPE: // Esc.
					this -> keyStates[ GameKeys::ESCAPE ] = false;
					break;

				default:
					break;
			}
		}
		
		//On window exit (X).
		else if ( this -> sdlEvent.type == SDL_QUIT )
		{
			signalExit();
		}

	} while(pendingEvent != 0);
}

/**
* @return InputHandler::keyStates
*/
std::array<bool, GameKeys::MAX> InputHandler::getKeyStates ()
{
	return this -> keyStates;
}

/**
* @return InputHandler::quitFlag
*/
bool InputHandler::isQuitFlag ()
{
	return this -> quitFlag;
}

/**
* Sets InputHandler::quitFlag to true.
*/
void InputHandler::signalExit ()
{
	this -> quitFlag = true;
}
/**
* Sets InputHandler::keyStates to false.
*/
void InputHandler::clearKey ( const GameKeys key_ )
{
	this -> keyStates.at( key_ ) = false;
}
