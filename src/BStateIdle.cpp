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
#include <assert.h>

#define POSITION_X 0
#define POSITION_Y 0
#define NUMBER_OF_IMAGES 7
#define TOTAL_TIME_ANIMATION 1

/**
* Shows the animation of the boss entering in idle.
*/
void BStateIdle::enter()
{
	Log( DEBUG ) << "STATE IDLE BOSS";

	// Changing the boss animation.
	this -> boss -> getAnimation() -> changeAnimation( POSITION_X, POSITION_Y, NUMBER_OF_IMAGES, false, TOTAL_TIME_ANIMATION );
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
void BStateIdle::update( const double DELTA_TIME )
{
	handle_boss_position();

	handle_boss_movements();
}

// Handle the boss position in the game.
void BStateIdle::handle_boss_position()
{
	if( this -> boss -> is_right && this -> boss -> x > 1960 )
	{
		this -> boss -> is_right = false;

	}else if( !this -> boss -> is_right && this -> boss -> x < 300 )
	{
		this -> boss -> is_right = true;

	}else
	{
		this -> boss -> is_right = false;
	}
}

// Handle the boss movements in the game.
void BStateIdle::handle_boss_movements()
{
	if( this -> boss -> is_right )
	{
		this -> boss -> move( false, true );

	}else
	{
		this -> boss -> move( true, false );
	}

	if( this -> boss -> saw_player )
	{
		this -> boss -> changeState( Boss::BStates::ATTACK );

	}else
	{
		Log( DEBUG ) << "Invalid boss movement!";
	}
}

/**
* The constructor.
* Initializes the attributes.
*/
BStateIdle::BStateIdle( Boss *const BOSS ) :
	StateBoss( BOSS )
{
	assert( BOSS );

}
