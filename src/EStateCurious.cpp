/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @EStateCurious.cpp
 * File responsible for implementing the maximum time Nadine can observe without being discovered.
 * Using the time elapsed.
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "EStateCurious.h"
#include "Logger.h"

#define INITIAL_ENEMY_SPEED 5.0
#define MAX_CURIOUS_TIME 4.666
#define ZERO 0

/*
 * Enter some data for animation
 * @see StateEnemy::enter
 */
void EStateCurious::enter()
{
	this -> enemy -> getAnimation() -> changeAnimation(1, ZERO, 9, false, 1.2);
	this -> enemy -> speed = INITIAL_ENEMY_SPEED;

	if ( enemy -> life <= ZERO)
	{
		enemy -> velocity_y_axis = ZERO;
		enemy -> changeState(Enemy::EStates::DEAD);
	} else {
		// No action.
	}
}

/*
 * @see StateEnemy::exit
 */
void EStateCurious::exit()
{

}

/*
 * Updates the data regarding the state of observation
 * @param dt : delta time (time elapsed)
 * @see StateEnemy::update
 */
void EStateCurious::update( const double DELTA_TIME)
{
	assert( DELTA_TIME >= ZERO);
	this -> time_elapsed += DELTA_TIME;

	// Aerial
	if ( !this -> enemy -> isGrounded )
	{
		this -> enemy -> changeState( Enemy::EStates::AERIAL );
		return;
	} else {
		// No Action.
	}

	if ( this -> enemy -> x - Enemy::px < 0.0)
	{
		this -> enemy -> velocity_x_axis += this -> enemy -> speed;
	} else {
		  this -> enemy -> velocity_x_axis -= this -> enemy -> speed;
	}

	if ( abs( this -> enemy->x - Enemy::px) < Enemy::alert_range && abs( this -> enemy->y-Enemy::py ) < Enemy::alert_range)
	{
		this -> enemy -> changeState(Enemy::EStates::ALERT);
		return;
	} else {
		// No Action.
	}

  //if the time elapsed is greater than the time observing the enemy returns to patrol.
	if ( time_elapsed >= MAX_CURIOUS_TIME )
	{
		assert( MAX_CURIOUS_TIME <= 4.666 );          		
		this -> enemy -> changeState(Enemy::EStates::PATROLLING);
		return;
	} else {
		// No Action.
	}
}

/*
 * The constructor.
 * @param enemy_ : Reference to the Enemy.
 */
EStateCurious::EStateCurious( Enemy* const enemy_) : StateEnemy(enemy_), time_elapsed(0.0)
{

}
