/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Enemy.cpp
 * File responsible for implementing the enemies of the game (except the boss).
 * They define the basic image and characteristics of the enemy, upgrade, destruction, collision among others.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "Enemy.h"
#include "Logger.h"
#include "LuaScript.h"
#include <cmath>
#include "EStateIdle.h"
#include "EStatePatrolling.h"
#include "EStateAerial.h"
#include "EStateCurious.h"
#include "EStateAlert.h"
#include "EStateAttack.h"
#include "EStateDead.h"
#include "Window.h"

#define ADD_STATE_EMPLACE(stateEnum, stateClass) this -> statesMap.emplace(stateEnum, new stateClass( this ))
#define ADD_STATE_INSERT(stateEnum, stateClass) this -> statesMap.insert(std::make_pair<EStates, StateEnemy*>(stateEnum, new stateClass( this )));

//Declaring and initialing position enemy in the origin of the x axis
double Enemy::px = 0.0;
//Declaring and initialing position enemy in the origin of the y axis
double Enemy::py = 0.0;
//Declaring the enemy with 3 life quantity
unsigned int Enemy::pLife = 3;
//Check if enemy is position vulnerable
bool Enemy::pVulnerable = false;
//Declaring the value for range dangerous of proximity for a enemy
double Enemy::alert_range = 300.0;
//Double value existing at pvulnerable
double Enemy::curious_range = 600.0;

/*
 * Method used to create all characteristics Enemy
 * @param x_ : Position on the x axis of the enemy
 * @param y_ : Position on the y axis of the enemy
 * @param path_ : The path to the desired sprite
 * @param patrol_ : Defines if the enemy patrols.
 * @param patrolLength_ : Defines the space traveled by the patrolman
 */
Enemy::Enemy( const double x_, const double y_, const std::string& path_, const bool patrol_,
	const double patrolLength_ ) : DynamicEntity(x_, y_, path_), original_X(x_), patrol(patrol_),
  patrol_length(patrolLength_), life(100), current_state(nullptr), animation(nullptr), statesMap(), dead(false)
{
	// Initialize all the states in Enemy.
	initializeStates();

	this -> speed = 3.0;

  //Getting information from lua script.
	LuaScript luaEnemy("lua/Enemy.lua");
	// Setting the level width/height.
	this -> width = luaEnemy.unlua_get<int>("enemy.dimensions.width");
	this -> height = luaEnemy.unlua_get<int>("enemy.dimensions.height");
	this -> animation = new Animation(0, 0, this -> width, this -> height, 0, false);

  //Condition to attribute the states map to current state.
	if ( this -> patrol )
	{
		this -> current_state = this -> statesMap.at(PATROLLING);
	} else {
		  this -> current_state = this -> statesMap.at(IDLE);
	  }

	this -> current_state -> enter();
}

/*
 * Method to destroy the enemy
 */
Enemy::~Enemy()
{
	//Attribute null to current state
	if ( this -> current_state != nullptr )
	{
		this -> current_state -> exit();
		this -> current_state = nullptr;
	}

  //Attribute null to animation
	if ( this -> animation != nullptr )
	{
    delete this -> animation;
    this -> animation = nullptr;
  }

	destroyStates();
}

/*
 * Method to update characteristics of the enemy
 * @param dt : delta time (time elapsed)
 */
void Enemy::update( const double dt_)
{
	//const double dt is passed as a parameter to know the time elapsed.
	this -> current_state -> update( dt_);
	forceMaxSpeed();

	scoutPosition( dt_);

	this -> animation -> update( this -> animationClip, dt_);

	updateBoundingBox();

  //Array to detect collision with enemy
	const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detectCollision();
	handleCollision(detections);

	updatePosition( dt_);
}

/*
 * Method to update characteristics of the enemy
 * @param cameraX_ : Define the locate for the camera on the x-axis
 * @param cameraY_ : Define the locate for the camera on the y-axis
 */
void Enemy::render( const double cameraX_, const double cameraY_)
{
	const double dx = this -> x - cameraX_;
	const double dy = this -> y - cameraY_;

	///////////////////////////////////////////////////////////////////////////////////////////
	// // Actual.
	// SDL_Rect actualRect = {(int)dx, (int)dy, (int)this->width, (int)this->height};
	// SDL_SetRenderDrawColor( Window::getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	// SDL_RenderFillRect(Window::getRenderer(), &actualRect);

	// Bounding box.
	// SDL_Rect boundingBox2 = {(int)(this->boundingBox.x - cameraX_), (int)(this->boundingBox.y - cameraY_), (int)this->boundingBox.w, (int)this->boundingBox.h};
	// SDL_SetRenderDrawColor( Window::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	// SDL_RenderFillRect(Window::getRenderer(), &boundingBox2);
	// ///////////////////////////////////////////////////////////////////////////////////////////

	if ( this -> sprite != nullptr )
	{
		SDL_RendererFlip flip = getFlip();

		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> sprite->render(dx - 120, dy, &this -> animationClip, false, 0.0, nullptr, flip);
		} else {
			  this -> sprite -> render(dx, dy, &this -> animationClip, false, 0.0, nullptr, flip);
		  }
	}
}

/*
 * Initialize all the states in Enemy here.
 */
void Enemy::initializeStates()
{
	ADD_STATE_INSERT(IDLE,         EStateIdle);
	ADD_STATE_INSERT(CURIOUS,      EStateCurious);
	ADD_STATE_INSERT(PATROLLING,   EStatePatrolling);
	ADD_STATE_INSERT(ALERT,        EStateAlert);
	ADD_STATE_INSERT(AERIAL,       EStateAerial);
	ADD_STATE_INSERT(ATTACK,       EStateAttack);
	ADD_STATE_INSERT(DEAD,         EStateDead);
}

/*
 * Delete all the states in Enemy here.
 */
void Enemy::destroyStates()
{
	std::map<EStates, StateEnemy*>::const_iterator it;
	for ( it = this -> statesMap.begin(); it != this -> statesMap.end(); it++ )
	{
		delete it->second;
	}
}

/*
 * Method used to exchange enemy of position
 * @param state_ : Define state enemy
 */
void Enemy::changeState( const EStates state_)
{
	this -> current_state -> exit();
	this -> current_state = this -> statesMap.at(state_);
	this -> current_state -> enter();
}

/*
 * Method used to check collision in enemy
 * @param detections : Pass an array of array containing the type of collision.
 */
void Enemy::handleCollision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_)
{
	//Collision on top
	if ( detections_.at(CollisionSide::SOLID_TOP) )
	{
		this -> vy = 0.0;
	}
	//Collision on bottom
	if ( detections_.at(CollisionSide::SOLID_BOTTOM) )
	{
		if ( this -> current_state == this -> statesMap.at(EStates::AERIAL) || this -> current_state == this -> statesMap.at
			(EStates::DEAD))
		{
			this -> nextY -= fmod( this -> nextY, 64.0 ) - 16.0;
			this -> vy = 0.0;
			if ( this -> isDead() )
			{
				this -> changeState(EStates::DEAD);
			}
			if ( this -> patrol )
			{
				this -> changeState(EStates::PATROLLING);
			} else {
				  this -> changeState(EStates::IDLE);
				  return;
			  }
		}
	} else {
		  if ( this -> current_state != this -> statesMap.at(EStates::AERIAL) )
		  {
			  changeState(EStates::AERIAL);
		  }
	  }
	//Collision on left
	if ( detections_.at(CollisionSide::SOLID_LEFT) )
	{
		this -> nextX = this -> x;
		this -> vx = 0.0;
	}
	//Collision on right
	if ( detections_.at(CollisionSide::SOLID_RIGHT) )
	{
		this -> nextX = this -> x;
		this -> vx = -0.001;
	}
}

/*
 * Designates maximum velocity on the x-axis and y
 */
void Enemy::forceMaxSpeed()
{
	this -> vx = ( this -> vx >= this -> maxSpeed) ? this -> maxSpeed : this -> vx ;
	this -> vy = ( this -> vy >= this -> maxSpeed) ? this -> maxSpeed : this -> vy ;
}

/*
 * Reference the specific animation.
 */
Animation *Enemy::getAnimation()
{
	return ( this -> animation );
}

/*
 * Setting enemy is dead
 * @param isDead_ : It says if it is alive or dead
 */
void Enemy::setDead( bool isDead_)
{
	this -> dead = isDead_;
}

/*
 * Inform that enemy is dead
 */
bool Enemy::isDead()
{
	return this -> dead;
}

/*
 * Method to delimit enemy dimensions update
 */
void Enemy::updateBoundingBox()
{
	//Bounding box at x position
	this -> boundingBox.x = (int) this -> nextX + 40;
	//Bounding box at y position
	this -> boundingBox.y = (int) this -> nextY + 40;
	//Bounding box at width
	this -> boundingBox.w = 150;
	//Bounding box at height
	this -> boundingBox.h = 200;
}
