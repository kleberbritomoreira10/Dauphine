/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateCrauching.cpp
* The state when the player is idle.
* License: Copyright (C) 2014 Alke Games.*/

#include "PStateCrouching.h"
#include "Logger.h"

/**
* Enter the Crouching state.
* @see StatePlayer::enter
*/
void PStateCrouching::enter()
{

	// Getting bounding box position parameters.
	this -> box.x = ( int )this -> player -> getWidth() / 4 - 33;
  	this -> box.w = ( int )this -> player -> getWidth() / 1.7;
	this -> box.y = ( int )this -> player -> getHeight() / 2;
	this -> box.h = ( int )this -> player -> getHeight() / 2;

	this -> player -> getAnimation() -> changeAnimation( 10,13,2,false,0.3 );
	this -> player -> isGrounded = true;
}

/**
* Exit the Crouching state.
* @see StatePlayer::exit
*/
void PStateCrouching::exit()
{
	this -> player -> getAnimation() -> changeAnimation( 1,14,2,false,0.3 );
}

/** 
* Handle the state inputs.
* @param KeyStates_: Boolean array that controls which keys are
					 pressed or not. 
* @see StatePlayer::handleInput
*/
void PStateCrouching::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	this -> player -> getAnimation() -> changeAnimation( 8,9,1,false,0 );

	// Getting idle.
	if ( !keyStates_[GameKeys::CROUCH] )
	{
		this -> player -> changeState( Player::PStates::IDLE );
		return;
	}

	this -> player -> slowVx();

	// Jump.
	if ( keyStates_[GameKeys::SPACE] )
	{
		this -> player -> jump();
		this -> player -> isGrounded = false;
		return;
	}

	// Moving player.
	if ( keyStates_[GameKeys::LEFT] || keyStates_[GameKeys::RIGHT] )
	{
		this -> player -> changeState( Player::PStates::MOVINGCROUCH );
		return;
	}

	// Rolling player.
	if ( keyStates_[GameKeys::ROLL] )
	{
		this -> player -> changeState( Player::PStates::ROLLING );
		return;
	}

}

/**
* The constructor.
* @param player_ : Reference to the player.
*/
PStateCrouching::PStateCrouching( Player *const player_ ) :
	StatePlayer( player_ )
{

}
