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

#define POSITION_X 0
#define POSITION_Y 0
#define NUMBER_OF_IMAGES 1
#define TOTAL_TIME 0

/**
* Shows the animation of the boss getting a shield.
*/
void BStateShield::enter()
{
	Log( DEBUG ) << "STATE SHIELD BOSS";

	// Change the animation.
	this -> boss -> getAnimation() -> changeAnimation( POSITION_X, POSITION_Y, NUMBER_OF_IMAGES, false, TOTAL_TIME );

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

	this -> boss -> changeState( Boss::BStates::IDLE ); // Set the state idle to the boss.
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateShield::BStateShield( Boss *const BOSS ) :
	StateBoss( BOSS )
{

}
