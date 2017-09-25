/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @PStateMovingCrouch.cpp
 * The state when the player is idle.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "PStateMovingCrouch.h"
#include "Logger.h"

/**
* Method called on load.
* Enter the Moving of Crouching
* @see StatePlayer::enter
*/
void PStateMovingCrouch::enter ()
{

	this -> box.x = ( int ) this -> player -> getWidth () / 4 - 33;
  this -> box.w = ( int ) this -> player -> getWidth () / 1.7;
	this -> box.y = ( int ) this -> player->getHeight () / 2;
	this -> box.h = ( int ) this -> player -> getHeight () / 2;

  // Changes the previous animation to crouch.
	this -> player -> getAnimation () -> changeAnimation ( 3, 10, 6, true, 1 );
	this -> player -> isGrounded = true;
	this -> player -> maxSpeed /= 2;
	this -> player -> speed = 15;

}

/**
* Method called on unload.
* @see StatePlayer::exit
*/
void PStateMovingCrouch::exit ()
{
	this -> player -> maxSpeed *= 2;
	this -> player -> speed = 20;
}

/**
* Update method for the state.
* While the Player is on the state, this method runs every update.
* @param keyStates_ : Boolean array that contains all input data.
*/
void PStateMovingCrouch::handleInput ( const std::array < bool, GameKeys::MAX > keyStates_ )
{

  // Check whether the character is crouched or not, if not, goes to idle
	if ( !keyStates_ [ GameKeys::CROUCH ] )
	{
		this -> player -> changeState ( Player::PStates::IDLE );
		return;
	}

	this -> player -> slowVx ();

	// Check if the character has stopped moving, if he is, he will crouch.
    if ( !keyStates_ [ GameKeys::LEFT ] && !keyStates_ [ GameKeys::RIGHT ] )
    {
        this -> player -> changeState ( Player::PStates::CROUCHING );
        return;
    }

	// Checks if the space key has been pressed and the character jumps.
	if ( keyStates_ [ GameKeys::SPACE ] )
	{
		this -> player -> jump ();
		this -> player -> isGrounded = false;
		return;
	}

	this -> player -> move ( keyStates_ [ GameKeys::LEFT ], keyStates_ [ GameKeys::RIGHT ] );

	if ( keyStates_ [ GameKeys::ROLL ] )
	{
		this -> player -> changeState ( Player::PStates::ROLLING );
		return;
	}
}

// @param player_ : Reference to the player.
PStateMovingCrouch::PStateMovingCrouch ( Player *const player_ ) :
	StatePlayer( player_ )
{

}
