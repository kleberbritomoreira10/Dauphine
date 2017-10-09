#include "EStatePatrolling.h"
#include <cfloat>
#include "Logger.h"

void EStatePatrolling::enter(){
	this->enemy->isGrounded = true;
	this->enemy->x = this->enemy->original_X;
	this->direction = 1.0;

	if(enemy->life <= 0){
		enemy->velocity_y_axis = 0;
		enemy->changeState(Enemy::EStates::DEAD);
	}
}

void EStatePatrolling::exit(){

}

void EStatePatrolling::update(const double DELTA_TIME){

	((void)DELTA_TIME); // Unused.
	
	// Patrol.
	if(abs(this->enemy->x - this->enemy->original_X) > this->enemy->patrol_length){
		// right
		if(this->enemy->x - this->enemy->original_X < 0.0){
			this->direction = 1.0;
		}
		// left
		else{
			this->direction = -1.0;
		}
		
	}
	else{
		// Do nothing.
	}

	this->enemy->velocity_x_axis += this->enemy->speed * this->direction;

	/// @todo Make the range be only in the direciton the enemy is facing.
	if(abs(this->enemy->x - Enemy::px) < Enemy::alert_range && abs(this->enemy->y - Enemy::py) < Enemy::alert_range){
		this->enemy->changeState(Enemy::EStates::ALERT);
		return;
	}
	else if(abs(this->enemy->x - Enemy::px) < Enemy::curious_range && abs(this->enemy->y - Enemy::py) < Enemy::curious_range){
		this->enemy->changeState(Enemy::EStates::CURIOUS);
		return;
	}
	
}

EStatePatrolling::EStatePatrolling(Enemy* const enemy_) :
	StateEnemy(enemy_),
	direction(0.0)
{

}
