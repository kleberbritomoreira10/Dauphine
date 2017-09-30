/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @EStateDead.cpp
 * File responsible for implementing the dead state.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "EStateDead.h"
#include "Logger.h"

double deadTime;

/*
 * Informs the dead state
 * @see StateEnemy::enter
 */
void EStateDead::enter()
{
	this -> enemy -> life = 0;
	this -> enemy -> set_dead(true);
	this -> enemy -> get_animation() -> change_animation(1, 1, 1, false, 0);
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
void EStateDead::update( const double DELTA_TIME)
{
	deadTime += DELTA_TIME;
	if ( deadTime >= 2 )
	{
		this -> enemy -> set_dead(true);
	}
	this -> enemy -> velocity_x_axis = 0;
}

/*
 * The constructor.
 * @param enemy_ : Reference to the Enemy.
 */
EStateDead::EStateDead(Enemy* const enemy_) : StateEnemy(enemy_)
{

}
