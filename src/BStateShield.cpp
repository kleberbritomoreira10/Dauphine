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
	// Define settings when the boss enter in the shield state.
	Log( DEBUG ) << "STATE SHIELD BOSS";
	this -> boss -> getAnimation() -> changeAnimation( 0, 0, 1, false, 0 );
	this->boss->hasShield = true;
}

/**
* Shows the boss losing a shield.
*/
void BStateShield::exit()
{

}

/**
* Changing the state of the boss shield.
* @param dt_ : The difference of the time betwen two events.
*/
void BStateShield::update( const double dt_ )
{
	( ( void )dt_ ); // Unused.

	this -> boss -> changeState( Boss::BStates::IDLE );
}

/**
* The constructor.
* Initializes the attributes.
* @param boss_ : The boss of the level of the game.
*/
BStateShield::BStateShield( Boss* const boss_ ) :
	StateBoss( boss_ )
{

}
