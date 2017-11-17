/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @EStateDead.cpp
 * File responsible for implementing the dead state.
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "EStateDead.h"
#include "Logger.h"

#define MINIMUN_DEAD_TIME 2
#define ZERO 0

/*
 * Informs the dead state
 * @see StateEnemy::enter
 */
void EStateDead::enter()
{
	this -> enemy -> life = ZERO;
	this -> enemy -> set_dead(true);
	this -> enemy -> getAnimation() -> changeAnimation(1, 1, 1, false, ZERO);
}

/*
 * @see StateEnemy::exit
 */
void EStateDead::exit()
{

}

/*
 * Update the status of the dead state
 * @param DELTA_TIME : delta time (time elapsed)
 * @see StateEnemy::update
 */

double dead_time = ZERO; // Informs the state of death through delta time 

void EStateDead::update( const double DELTA_TIME)
{
	assert (DELTA_TIME >= ZERO );
	assert ( dead_time >= ZERO );

	dead_time += DELTA_TIME;
	if ( dead_time >= MINIMUN_DEAD_TIME )
	{
		this -> enemy -> set_dead(true);
	} else {
		this -> enemy -> set_dead(false);
	}
	this -> enemy -> velocity_x_axis = ZERO;
}

/*
 * The constructor.
 * @param enemy_ : Reference to the Enemy.
 */
EStateDead::EStateDead(Enemy* const enemy_) : StateEnemy(enemy_)
{

}
