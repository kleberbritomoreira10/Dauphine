/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @EStateIdle.cpp
 * File responsible for implementing idle state.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "EStateIdle.h"
#include <cmath>
#include "SDLWrapper.h"
#include "Logger.h"

/*
 * Check if the enemy is lying down, if yes declares him dead.
 * @see StateEnemy::enter
 */
void EStateIdle::enter()
{
	this -> enemy -> isGrounded = true;

	if ( enemy -> life <= 0 )
	{
		enemy -> vy = 0;
		enemy -> changeState( Enemy::EStates::DEAD );
	}
}

/*
 * @see StateEnemy::exit
 */
void EStateIdle::exit()
{

}

/*
 * Update the datas for estate idle
 * @param dt_ : delta time (time elapsed)	
 * @see StateEnemy::update
 */
void EStateIdle::update( const double dt_)
{
	((void)dt_); // Unused.
	
	// Aerial
	if ( !this -> enemy -> isGrounded )
	{
		this -> enemy -> changeState( Enemy::EStates::AERIAL );
		return;
	}

	/// @todo Make the range be only in the direciton the enemy is facing.
	if ( abs ( this -> enemy->x - Enemy::px) < Enemy::alertRange )
	{
		this -> enemy -> changeState(Enemy::EStates::ALERT);
		return;
	}
	//Check the range to change to the curious state.
	else if ( abs ( this -> enemy->x - Enemy::px) < Enemy::curiousRange )
	{
		this -> enemy -> changeState(Enemy::EStates::CURIOUS);
		return;
	}
}

/*
 * The constructor.
 * @param enemy_ : Reference to the Enemy.
 */
EStateIdle::EStateIdle( Enemy* const enemy_) : StateEnemy(enemy_)
{

}
