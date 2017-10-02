#include "EStateAlert.h"
#include "Logger.h"

void EStateAlert::enter(){
	this->enemy->getAnimation()->changeAnimation(1, 0, 9, false, 0.9);
	this->enemy->speed = 6.5;

	if(enemy->life <= 0){
		enemy->vy = 0;
		enemy->changeState(Enemy::EStates::DEAD);
	}
}

void EStateAlert::exit(){
}

void EStateAlert::update(const double dt_){

	((void)dt_); // Unused.
	
	// Aerial
	if(!this->enemy->isGrounded){
		this->enemy->changeState(Enemy::EStates::AERIAL);
		return;
	}

	if(this->enemy->x - Enemy::px < 0.0){
		this->enemy->vx += this->enemy->speed;

	}
	else{
		this->enemy->vx -= this->enemy->speed;
	}

	if(abs(this->enemy->x - Enemy::px) < 100){
		if(abs(this->enemy->y - Enemy::py) < 200){
			this->enemy->changeState(Enemy::EStates::ATTACK);
		}
	}

	if(!(abs(this->enemy->x - Enemy::px) < Enemy::alert_range*2 && abs(this->enemy->y - Enemy::py) < Enemy::alert_range)){
		this->enemy->changeState(Enemy::EStates::PATROLLING);
		return;
	}	
}

EStateAlert::EStateAlert(Enemy* const enemy_) :
	StateEnemy(enemy_)
{

}
