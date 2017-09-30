#include "EStateAttack.h"
#include "Logger.h"

double attackTime; // NO

void EStateAttack::enter(){
	this->enemy->get_animation()->change_animation(2, 1, 6, false, 0.6);
	this->enemy->speed = 7.0;
	attackTime = 0;

	if(enemy->life <= 0){
		enemy->velocity_y_axis = 0;
		enemy->change_state(Enemy::EStates::DEAD);
	}
}

void EStateAttack::exit(){

}

void EStateAttack::update(const double DELTA_TIME){

	attackTime += DELTA_TIME;
	const double attackTotalTime = 0.6;

	/// @todo Refactor the way the enemy hurts the player.
	
	if(this->enemy->velocity_x_axis >= 0){
		this->enemy->velocity_x_axis = 0;
	}
	else{
		this->enemy->velocity_x_axis = -0.001; //Set this value so that the Enemy won't Flip when attacking
	}

	if(attackTime > attackTotalTime){
		if(Enemy::position_vulnerable){
			Enemy::points_life--;
		}
		this->enemy->change_state(Enemy::EStates::ALERT);
	}
}

EStateAttack::EStateAttack(Enemy* const enemy_) :
	StateEnemy(enemy_)
{

}
