/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateDead.cpp
* The state when the player is idle.
* License: Copyright (C) 2014 Alke Games.*/

#include "PStateDead.h"
#include "Logger.h"
#include "Game.h"

/**
* Enter in the dead state.
* @see StatePlayer::enter
*/
void PStateDead::enter()
{
	Log( DEBUG )  << "STATE DEAD";

	this -> box.x = 58;
	this -> box.y = 72;
	this -> box.w = 130;
	this -> box.h = 160;

	this -> player -> getAnimation()  -> changeAnimation( 8, 4, 1, false, 0 ) ;
	this -> player -> isGrounded = false;
}

/**
* Exit the dead state.
* @see StatePlayer::exit
*/
void PStateDead::exit()
{

}

/**
* Handle the user inputs.
  @param KeyStates_: Boolean array that controls which keys are
					 pressed or not. 
* @see StatePlayer::handleInput
*/
void PStateDead::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	( ( void )  keyStates_ ) ; // Unused.
	this -> player -> applyGravity() ;
}

/**
* The constructor.
* @param player_ : Reference to the player.
*/
PStateDead::PStateDead( Player *const player_ )  :
	StatePlayer( player_ )
{

}
