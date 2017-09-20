/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateIdle.cpp
* The state of the idle of the boss.
* Class that controll the idle of the boss.
* License: Copyright (C) 2014 Alke Games.
*/

#include "BStateIdle.h"
#include "Logger.h"

/**
* Shows the animation of the boss entering in idle.
*/
void BStateIdle::enter()
{
	Log( DEBUG ) << "STATE IDLE BOSS";
	this -> boss -> getAnimation() -> changeAnimation( 0, 0, 7, false, 1 );
}

/**
* Ends with idleness of the boss.
*/
void BStateIdle::exit()
{

}

/**
* Changing the static position of the boss in idle.
* @param dt_ : The difference of the time betwen two events.
*/
void BStateIdle::update( const double dt_ )
{
	( ( void )dt_ ); // Unused.

	// Defining the new position of the boss based in the current position
	if( this -> boss -> isRight && this -> boss -> x > 1960 )
	{
		this -> boss -> isRight = false;
	}

	// Defining the new position of the boss based in the current position
	else if( !this -> boss -> isRight && this -> boss -> x < 300 )
	{
		this -> boss -> isRight = true;
	}
	
	// Defining the new position of the boss based in the current position
	if( this -> boss -> isRight )
	{
		this -> boss -> move( false, true );
	}

	// Defining the new position of the boss based in the current position	
	else
	{
		this -> boss -> move( true, false );
	}

	// Defining the new position of the boss based in the current position	
	if( this -> boss -> sawPlayer )
	{
		this -> boss -> changeState( Boss::BStates::ATTACK );
	}

}

/**
* The constructor.
* Initializes the attributes.
* @param boss_ : The boss of the level of the game.
*/
BStateIdle::BStateIdle( Boss* const boss_ ) :
	StateBoss( boss_ )
{

}
