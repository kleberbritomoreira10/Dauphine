/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @EStateCurious.cpp
 * File responsible for implementing the maximum time Nadine can observe without being discovered. 
 * Using the time elapsed.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "EStateCurious.h"
#include "Logger.h"

#define MAX_CURIOUS_TIME 4.666

/*
 * Enter some data for animation
 * @see StateEnemy::enter
 */
void EStateCurious::enter()
{
	this -> enemy -> getAnimation() -> changeAnimation(1, 0, 9, false, 1.2);
	this -> enemy -> speed = 5.0;

	if ( enemy -> life <= 0)
	{
		enemy -> vy = 0;
		enemy -> changeState(Enemy::EStates::DEAD);
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
void EStateCurious::update( const double dt_)
{

	this -> timeElapsed += dt_;

	// Aerial
	if ( !this -> enemy -> isGrounded )
	{
		this -> enemy -> changeState( Enemy::EStates::AERIAL );
		return;
	}
  
	if ( this -> enemy -> x - Enemy::px < 0.0)
	{
		this -> enemy -> vx += this -> enemy -> speed;
	} else {
		  this -> enemy -> vx -= this -> enemy -> speed;
	  }
 
	if ( abs( this -> enemy->x - Enemy::px) < Enemy::alertRange && abs( this -> enemy->y-Enemy::py ) < Enemy::alertRange)
	{
		this -> enemy -> changeState(Enemy::EStates::ALERT);
		return;
	}
  
  //if the time elapsed is greater than the time observing the enemy returns to patrol.
	if ( timeElapsed >= MAX_CURIOUS_TIME )
	{
		this -> enemy -> changeState(Enemy::EStates::PATROLLING);
		return;
	}
}

/*
 * The constructor.
 * @param enemy_ : Reference to the Enemy.
 */
EStateCurious::EStateCurious( Enemy* const enemy_) : StateEnemy(enemy_), timeElapsed(0.0)
{

}
