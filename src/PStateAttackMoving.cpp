/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @PStateAttackMoving.cpp
 * The state when the player attack moving.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "PStateAttackMoving.h"
#include "Logger.h"

/*
Initializes all attributes and character animation
@see StatePlayer::enter
*/
void PStateAttackMoving::enter ()
{

	this -> box.x = 58;
	this -> box.y = 72;
	this -> box.w = 130;
	this -> box.h = 160;

	this -> player -> getAnimation () -> changeAnimation ( 5, 8, 9, false, 0.7 );
}

void PStateAttackMoving::exit ()
{
	this -> player -> canAttack = true;
}

void PStateAttackMoving::handleInput ( const std::array < bool, GameKeys::MAX > keyStates_ )
{
	this -> player -> move ( keyStates_ [ GameKeys::LEFT ], keyStates_ [ GameKeys::RIGHT ] );

	// When the character is in motion, it checks to see if a given frame has been reached to change the animation
	if ( this -> player -> getAnimation () -> getCurrentFrame () == 9 )
	{
		this -> player -> changeState ( Player::PStates::MOVING );
	}
}

// @param player_ : Reference to the player.
PStateAttackMoving::PStateAttackMoving ( Player *const player_ ) :
	StatePlayer ( player_ )
{

}
