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

void EStateDead::enter()
{
	this -> enemy -> life = 0;
	this -> enemy -> setDead(true);
	this -> enemy -> getAnimation() -> changeAnimation(1, 1, 1, false, 0);
}

void EStateDead::exit()
{

}

void EStateDead::update( const double dt_)
{
	deadTime += dt_;
	if ( deadTime >= 2 )
	{
		this -> enemy -> setDead(true);
	}
	this -> enemy -> vx = 0;
}

EStateDead::EStateDead(Enemy* const enemy_) : StateEnemy(enemy_)
{

}
