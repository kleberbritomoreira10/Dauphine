
#include "EStateAlert.h"
#include "Logger.h"

void EStateAlert::enter(){
	this->enemy->get_animation()->change_animation(1, 0, 9, false, 0.9);
	this->enemy->speed = 6.5;

	if(enemy->life <= 0){
		enemy->velocity_y_axis = 0;
		enemy->change_state(Enemy::EStates::DEAD);
	}
}

void EStateAlert::exit(){
}

void EStateAlert::update(const double DELTA_TIME){

	((void)DELTA_TIME); // Unused.
	
	// Aerial
	if(!this->enemy->isGrounded){
		this->enemy->change_state(Enemy::EStates::AERIAL);
		return;
	}

	if(this->enemy->x - Enemy::px < 0.0){
		this->enemy->velocity_x_axis += this->enemy->speed;

	}
	else{
		this->enemy->velocity_x_axis -= this->enemy->speed;
	}

	if(abs(this->enemy->x - Enemy::px) < 100){
		if(abs(this->enemy->y - Enemy::py) < 200){
			this->enemy->change_state(Enemy::EStates::ATTACK);
		}
	}

	if(!(abs(this->enemy->x - Enemy::px) < Enemy::alert_range*2 && abs(this->enemy->y - Enemy::py) < Enemy::alert_range)){
		this->enemy->change_state(Enemy::EStates::PATROLLING);
		return;
	}	
}

EStateAlert::EStateAlert(Enemy* const enemy_) :
	StateEnemy(enemy_)
{

}
