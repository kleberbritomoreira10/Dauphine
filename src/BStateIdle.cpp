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
*/
void BStateIdle::update( const double dt_ )
{
	( ( void )dt_ ); // Unused.

	if( this -> boss -> isRight && this -> boss -> x > 1960 )
	{
		this -> boss -> isRight = false;
	}
	else if( !this -> boss -> isRight && this -> boss -> x < 300 )
	{
		this -> boss -> isRight = true;
	}
	
	if( this -> boss -> isRight )
	{
		this -> boss -> move( false, true );
	}
	else
	{
		this -> boss -> move( true, false );
	}

	if( this -> boss -> sawPlayer )
	{
		this -> boss -> changeState( Boss::BStates::ATTACK );
	}

}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIdle::BStateIdle( Boss* const boss_ ) :
	StateBoss( boss_ )
{

}
