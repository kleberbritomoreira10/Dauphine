/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateAttackJumping.cpp
* The state when the player is idle.
* License: Copyright (C) 2014 Alke Games.*/

#include "PStateAttackJumping.h"
#include "Logger.h"
#include "Game.h"

/**
* Enter de Attack jumping state.
* @see StatePlayer::enter
*/
void PStateAttackJumping::enter()
{

   	this -> box.x = 58;
    this -> box.y = 72;
    this -> box.w = 130;
    this -> box.h = 145;

    this -> player -> getAnimation() -> changeAnimation( 3, 9, 7, false, 0.4 );
}

/**
* Exit the Sttack jumping state.
* @see StatePlayer::exit
*/
void PStateAttackJumping::exit()
{
	Log( DEBUG ) << "EXIT STATE ATTACK JUMPING";
	this -> player -> canAttack = true;
}

/**
* Handle the user inputs.
* @param KeyStates_: Boolean array that controls which keys are
					 pressed or not. 
* @see StatePlayer::handleInput
*/
void PStateAttackJumping::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	Log( DEBUG ) << "STATE ATTACK JUMPING";

	// Changing the state.
	if ( this -> player -> getAnimation() -> getCurrentFrame() == 7 )
  	{
		this -> player -> changeState( Player::PStates::AERIAL );
	}
	// Applying gravity.
	this -> player -> applyGravity();

	// Move ( while on air ).
    this -> player -> move( keyStates_[GameKeys::LEFT], keyStates_[GameKeys::RIGHT] );
}

/**
* The constructor.
* @param player_ : Reference to the player.
*/
PStateAttackJumping::PStateAttackJumping( Player *const player_ ) :
	StatePlayer( player_ )
{

}
