/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateAttack.cpp
* The state when the player able to attack.
* License: Copyright (C) 2014 Alke Games.*/

#include "BStateAttack.h"
#include "Logger.h"
#include "Game.h"

#include <cstdlib>
#include <ctime>

int attackIndex = 0;

/**
* Enter the State Attack.
* @see StateBoss::enter
*/
void BStateAttack::enter()
{
	Log( DEBUG ) << "STATE ATTACK BOSS";
	this -> boss -> getAnimation() -> changeAnimation( 0, 0, 7, false, 1 );
	this -> boss -> sawPlayer = false;
	attackIndex = rand()%4;
}

/**
* Exit the State Attack.
* @see StateBoss::enter
*/
void BStateAttack::exit()
{
	this -> boss -> canWalk = false;
}

/**
* Update the state.
* @param dt_: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
* @see StateBoss::update
*/
void BStateAttack::update( const double dt_ )
{
	( ( void )dt_ ); // Unused.

	if ( attackIndex == 3 )
	{
		this -> boss -> changeState( Boss::BStates::SHIELD );
	}
	else if ( attackIndex == 0 )
	{
		this -> boss -> changeState( Boss::BStates::TELEPORT );
	}
	else if ( attackIndex == 2 )
	{
		this -> boss -> changeState( Boss::BStates::ICEPRISION );
	}
	else if ( attackIndex == 1 )
	{
		this -> boss -> changeState( Boss::BStates::MAGICPROJECTILE );
	} else
	{

	}
}

/**
* The constructor.
* @param boss_ : Reference to the Boss.
*/
BStateAttack::BStateAttack( Boss *const boss_ ) :
	StateBoss( boss_ )
{
	srand( time( nullptr ) );
}
