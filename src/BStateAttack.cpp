
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
	this -> boss -> get_animation() -> change_animation( 0, 0, 7, false, 1 );
	this -> boss -> saw_player = false;
	attackIndex = rand()%4;
}

/**
* Exit the State Attack.
* @see StateBoss::enter
*/
void BStateAttack::exit()
{
	this -> boss -> can_walk = false;
}

/**
* Update the state.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
* @see StateBoss::update
*/
void BStateAttack::update( const double DELTA_TIME )
{
	( ( void )DELTA_TIME ); // Unused.

	if ( attackIndex == 3 )
	{
		this -> boss -> change_state( Boss::BStates::SHIELD );
	}
	else if ( attackIndex == 0 )
	{
		this -> boss -> change_state( Boss::BStates::TELEPORT );
	}
	else if ( attackIndex == 2 )
	{
		this -> boss -> change_state( Boss::BStates::ICEPRISION );
	}
	else if ( attackIndex == 1 )
	{
		this -> boss -> change_state( Boss::BStates::MAGICPROJECTILE );
	} else
	{

	}
}

/**
* The constructor.
* @param BOSS : Reference to the Boss.
*/
BStateAttack::BStateAttack( Boss *const BOSS ) :
	StateBoss( BOSS )
{
	srand( time( nullptr ) );
}
