/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateShield.cpp
* The state of the shield of the boss.
* Class that controll the shield of the boss.
* License: Copyright (C) 2014 Alke Games.
*/

#include "BStateShield.h"
#include "Logger.h"

/**
* Shows the animation of the boss getting a shield.
*/
void BStateShield::enter()
{
	Log( DEBUG ) << "STATE SHIELD BOSS";
	this -> boss -> getAnimation() -> changeAnimation( 0, 0, 1, false, 0 );
	this -> boss -> has_shield = true;
}

/**
* Shows the boss losing a shield.
*/
void BStateShield::exit()
{

}

/**
* Changing the state of the boss shield.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void BStateShield::update( const double DELTA_TIME )
{
	( ( void )DELTA_TIME ); // Unused.

	this -> boss -> changeState( Boss::BStates::IDLE );
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateShield::BStateShield( Boss *const BOSS ) :
	StateBoss( BOSS )
{

}
