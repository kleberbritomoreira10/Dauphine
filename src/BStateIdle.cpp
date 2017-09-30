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
	this -> boss -> get_animation() -> change_animation( 0, 0, 7, false, 1 );
}

/**
* Ends with idleness of the boss.
*/
void BStateIdle::exit()
{

}

/**
* Changing the static position of the boss in idle.
* @param DELTA_TIME: Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void BStateIdle::update( const double DELTA_TIME )
{
	( ( void )DELTA_TIME ); // Unused.

	if( this -> boss -> is_right && this -> boss -> x > 1960 )
	{
		this -> boss -> is_right = false;
	}
	else if( !this -> boss -> is_right && this -> boss -> x < 300 )
	{
		this -> boss -> is_right = true;
	}
	
	if( this -> boss -> is_right )
	{
		this -> boss -> move( false, true );
	}
	else
	{
		this -> boss -> move( true, false );
	}

	if( this -> boss -> saw_player )
	{
		this -> boss -> change_state( Boss::BStates::ATTACK );
	}

}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIdle::BStateIdle( Boss* const BOSS ) :
	StateBoss( BOSS )
{

}
