/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @ControllerHandler.cpp
* The input handler for joysticks.
* License: Copyright (C) 2014 Alke Games.*/

#include "ControllerHandler.h"
#include "Logger.h"

#include <iostream>

/**
* The constructor.
* Verifies if there is a joystick present.
*/
ControllerHandler::ControllerHandler() :
    gameController( nullptr )
{

    // Loading controller
	if ( SDL_NumJoysticks() > 0 ){
         this -> gameController = SDL_GameControllerOpen( 0 );
 	}

    // Vreifying if there are any activated controller.
	if ( this -> gameController == nullptr ){
		Log(INFO) << "Unable to find a controller, defaulting to keyboard.";
    }

    this -> keyStates.fill( false );
}

/**
* Handles controller the input.
* Detects the pending events, and simulates an appropriate KeyEvent by pushing one into
*   a stack.
* @param sdlEvent_ : The SDL internal event structure.
*/
void ControllerHandler::handleInput( SDL_Event& sdlEvent_ )
{

    this -> keyStates[ GameKeys::SPACE ]  = false;
    this -> keyStates[ GameKeys::ROLL ]  = false;
    this -> keyStates[ GameKeys::LATTACK ]  = false;
    this -> keyStates[ GameKeys::NLATTACK ]  = false;
    this -> keyStates[ GameKeys::ACTION ]  = false;

    static int pressed = 0;

    // Handling controller inputs.
    if ( sdlEvent_.type == SDL_CONTROLLERBUTTONDOWN )
    {

        switch( sdlEvent_.cbutton.button )
        {
            case controllerMap::buttons::FACE_DOWN: // Jump.
                if ( !( pressed & ( 1 << controllerMap::buttons::FACE_DOWN ) ) )
                {
                    this -> keyStates[ GameKeys::SPACE ] = true;
                    pressed |= ( 1 << controllerMap::buttons::FACE_DOWN );
                }
            break;

 			case controllerMap::buttons::FACE_UP: // Action.
                if (!(pressed & (1 << controllerMap::buttons::FACE_UP)))
                {
                    this -> keyStates[ GameKeys::ACTION ] = true;
                    pressed |= (1 << controllerMap::buttons::FACE_UP);
                }
                break;

            case controllerMap::buttons::DUP: // Move Up.
                this -> keyStates[ GameKeys::UP ] = true;
                break;

            case controllerMap::buttons::DDOWN: // Move Down.
                this -> keyStates[ GameKeys::DOWN ] = true;
            	break;

            case controllerMap::buttons::DLEFT: // Move Left.
                this -> keyStates[ GameKeys::LEFT ] = true;
                break;

            case controllerMap::buttons::DRIGHT: // Move Right.
                this -> keyStates[ GameKeys::RIGHT ] = true;
            break;

            case controllerMap::buttons::RDTRIGGER: // Roll
                if (!(pressed & (1 << controllerMap::buttons::RDTRIGGER)))
                {
                    this -> keyStates[ GameKeys::ROLL ] = true;
                    pressed |= (1 << controllerMap::buttons::RDTRIGGER);
                }
            break;

            case controllerMap::buttons::LDTRIGGER: // Crouch
                this -> keyStates[ GameKeys::CROUCH ] = true;
            break;

            case controllerMap::buttons::FACE_LEFT: // Lethal Attack
                 if (!(pressed & (1 << controllerMap::buttons::FACE_LEFT)))
                 {
                    this -> keyStates[ GameKeys::LATTACK ] = true;
                    pressed |= (1 << controllerMap::buttons::FACE_LEFT);
                }
            break;

            case controllerMap::buttons::FACE_RIGHT: // Lethal Attack
                if ( ! ( pressed & ( 1 << controllerMap::buttons::FACE_RIGHT ) ) )
                {
                    this -> keyStates[ GameKeys::LATTACK ] = true;
                    pressed |= ( 1 << controllerMap::buttons::FACE_RIGHT );
                }
            break;

            case controllerMap::buttons::START: // Options
                this -> keyStates[ GameKeys::ESCAPE ] = true;
            break;

            default:
                break;
        }

    }

    // On keyup.
    if ( sdlEvent_.type == SDL_CONTROLLERBUTTONUP )
    {

        switch( sdlEvent_.cbutton.button )
        {

            case controllerMap::buttons::FACE_DOWN: // Jump.
                this -> keyStates[ GameKeys::SPACE ] = false;
                pressed &= ~(1 << controllerMap::buttons::FACE_DOWN);
            break;

            case controllerMap::buttons::FACE_UP: // Action.
                this -> keyStates[ GameKeys::ACTION ] = false;
                pressed &= ~(1 << controllerMap::buttons::FACE_UP);
                break;

            case controllerMap::buttons::DUP: // Move Up.
                this -> keyStates[ GameKeys::UP ] = false;
                break;

            case controllerMap::buttons::DDOWN: // Move Down.
                this -> keyStates[ GameKeys::DOWN ] = false;
                break;

            case controllerMap::buttons::DLEFT: // Move Left.
                this -> keyStates[ GameKeys::LEFT ] = false;
                break;

            case controllerMap::buttons::DRIGHT: // Move Right.
                this -> keyStates[ GameKeys::RIGHT ] = false;
            break;

            case controllerMap::buttons::RDTRIGGER: // Roll
                this -> keyStates[ GameKeys::ROLL ] = false;
                pressed &= ~( 1 << controllerMap::buttons::RDTRIGGER );
            break;

            case controllerMap::buttons::LDTRIGGER: // Crouch
                this -> keyStates[ GameKeys::CROUCH ] = false;
            break;

            case controllerMap::buttons::FACE_LEFT: // Lethal Attack
                this -> keyStates[ GameKeys::LATTACK ] = false;
                pressed &= ~( 1 << controllerMap::buttons::FACE_LEFT );
            break;

            case controllerMap::buttons::FACE_RIGHT: // Lethal Attack
                this -> keyStates[ GameKeys::NLATTACK ] = false;
                pressed &= ~( 1 << controllerMap::buttons::FACE_RIGHT );
            break;

            case controllerMap::buttons::START: // Lethal Attack
                this -> keyStates[ GameKeys::ESCAPE ] = false;
            break;

            default:
                break;
        }
	}

    // Handling jey states.
	if ( sdlEvent_.type == SDL_CONTROLLERAXISMOTION ){

		switch( sdlEvent_.caxis.axis )
    {

			case controllerMap::axes::LATRIGGER:
					if ( sdlEvent_.caxis.value > TRIGGER_DEAD_ZONE )
          {
                        this -> keyStates[ GameKeys::AIM ] = true;
					}

					else{
					   this -> keyStates[ GameKeys::AIM ] = false;
                    }

				break;


            case controllerMap::axes::RATRIGGER:
                    if ( sdlEvent_.caxis.value > TRIGGER_DEAD_ZONE )
                    {
                        this -> keyStates[ GameKeys::ITEMS ] = true;
                    } else
                    {
                       this -> keyStates[ GameKeys::ITEMS ] = false;
                    }

                break;

			default:
				break;

		}
	}

}
