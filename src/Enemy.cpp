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

#define ADD_STATE_EMPLACE(stateEnum, stateClass) this -> states_map.emplace(stateEnum, new stateClass( this ))
#define ADD_STATE_INSERT(stateEnum, stateClass) this -> states_map.insert(std::make_pair<EStates, StateEnemy*>(stateEnum, new stateClass( this )));

//Declaring and initialing position enemy in the origin of the x axis
double Enemy::px = 0.0;
//Declaring and initialing position enemy in the origin of the y axis
double Enemy::py = 0.0;
//Declaring the enemy with 3 life quantity
unsigned int Enemy::points_life = 3;
//Check if enemy is position vulnerable
bool Enemy::position_vulnerable = false;
//Declaring the value for range dangerous of proximity for a enemy
double Enemy::alert_range = 300.0;
//Double value existing at pvulnerable
double Enemy::curious_range = 600.0;

/*
 * Method used to create all characteristics Enemy
 * @param x_ : Position on the x axis of the enemy
 * @param y_ : Position on the y axis of the enemy
 * @param PATH : The path to the desired sprite
 * @param patrol_ : Defines if the enemy patrols.
 * @param patrolLength_ : Defines the space traveled by the patrolman
 */
Enemy::Enemy( const double x_, const double y_, const std::string& PATH, const bool patrol_,
	const double patrolLength_ ) : DynamicEntity(x_, y_, PATH), original_X(x_), patrol(patrol_),
  patrol_length(patrolLength_), life(100), current_state(nullptr), animation(nullptr), states_map(), dead(false)
{
	// Initialize all the states in Enemy.
	initialize_states();

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
		this -> current_state = this -> states_map.at(PATROLLING);
	} else {
		  this -> current_state = this -> states_map.at(IDLE);
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

	destroy_states();
}

/*
 * Method to update characteristics of the enemy
 * @param dt : delta time (time elapsed)
 */
void Enemy::update( const double DELTA_TIME)
{
	//const double dt is passed as a parameter to know the time elapsed.
	this -> current_state -> update( DELTA_TIME);
	forceMaxSpeed();

	scout_position( DELTA_TIME);

	this -> animation -> update( this -> animation_clip, DELTA_TIME);

	update_bounding_box();

  //Array to detect collision with enemy
	const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detect_collision();
	handle_collision(detections);

	update_position( DELTA_TIME);
}

/*
 * Method to update characteristics of the enemy
 * @param camera_position_x : Define the locate for the camera on the x-axis
 * @param camera_position_y : Define the locate for the camera on the y-axis
 */
void Enemy::render( const double camera_position_x, const double camera_position_y)
{
	const double dx = this -> x - camera_position_x;
	const double dy = this -> y - camera_position_y;

	///////////////////////////////////////////////////////////////////////////////////////////
	// // Actual.
	// SDL_Rect actualRect = {(int)dx, (int)dy, (int)this->width, (int)this->height};
	// SDL_SetRenderDrawColor( Window::getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	// SDL_RenderFillRect(Window::getRenderer(), &actualRect);

	// Bounding box.
	// SDL_Rect boundingBox2 = {(int)(this->boundingBox.x - camera_position_x), (int)(this->boundingBox.y - camera_position_y), (int)this->boundingBox.w, (int)this->boundingBox.h};
	// SDL_SetRenderDrawColor( Window::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	// SDL_RenderFillRect(Window::getRenderer(), &boundingBox2);
	// ///////////////////////////////////////////////////////////////////////////////////////////

	if ( this -> sprite != nullptr )
	{
		SDL_RendererFlip flip = get_flip();

		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> sprite->render(dx - 120, dy, &this -> animation_clip, false, 0.0, nullptr, flip);
		} else {
			  this -> sprite -> render(dx, dy, &this -> animation_clip, false, 0.0, nullptr, flip);
		  }
	}
}

/*
 * Initialize all the states in Enemy here.
 */
void Enemy::initialize_states()
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
void Enemy::destroy_states()
{
	std::map<EStates, StateEnemy*>::const_iterator it;
	for ( it = this -> states_map.begin(); it != this -> states_map.end(); it++ )
	{
		delete it->second;
	}
}

/*
 * Method used to exchange enemy of position
 * @param state_ : Define state enemy
 */
void Enemy::change_state( const EStates state_)
{
	this -> current_state -> exit();
	this -> current_state = this -> states_map.at(state_);
	this -> current_state -> enter();
}

/*
 * Method used to check collision in enemy
 * @param detections : Pass an array of array containing the type of collision.
 */
void Enemy::handle_collision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_)
{
	//Collision on top
	if ( detections_.at(CollisionSide::SOLID_TOP) )
	{
		this -> velocity_y_axis = 0.0;
	}
	//Collision on bottom
	if ( detections_.at(CollisionSide::SOLID_BOTTOM) )
	{
		if ( this -> current_state == this -> states_map.at(EStates::AERIAL) || this -> current_state == this -> states_map.at
			(EStates::DEAD))
		{
			this -> nextY -= fmod( this -> nextY, 64.0 ) - 16.0;
			this -> velocity_y_axis = 0.0;
			if ( this -> is_dead() )
			{
				this -> change_state(EStates::DEAD);
			}
			if ( this -> patrol )
			{
				this -> change_state(EStates::PATROLLING);
			} else {
				  this -> change_state(EStates::IDLE);
				  return;
			  }
		}
	} else {
		  if ( this -> current_state != this -> states_map.at(EStates::AERIAL) )
		  {
			  change_state(EStates::AERIAL);
		  }
	  }
	//Collision on left
	if ( detections_.at(CollisionSide::SOLID_LEFT) )
	{
		this -> next_position_x = this -> x;
		this -> velocity_x_axis = 0.0;
	}
	//Collision on right
	if ( detections_.at(CollisionSide::SOLID_RIGHT) )
	{
		this -> next_position_x = this -> x;
		this -> velocity_x_axis = -0.001;
	}
}

/*
 * Designates maximum velocity on the x-axis and y
 */
void Enemy::forceMaxSpeed()
{
	this -> velocity_x_axis = ( this -> velocity_x_axis >= this -> maxSpeed) ? this -> maxSpeed : this -> velocity_x_axis ;
	this -> velocity_y_axis = ( this -> velocity_y_axis >= this -> maxSpeed) ? this -> maxSpeed : this -> velocity_y_axis ;
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
 * @param is_dead_ : It says if it is alive or dead
 */
void Enemy::set_dead( bool is_dead_)
{
	this -> dead = is_dead_;
}

/*
 * Inform that enemy is dead
 */
bool Enemy::is_dead()
{
	return this -> dead;
}

/*
 * Method to delimit enemy dimensions update
 */
void Enemy::update_bounding_box()
{
	//Bounding box at x position
	this -> boundingBox.x = (int) this -> next_position_x + 40;
	//Bounding box at y position
	this -> boundingBox.y = (int) this -> nextY + 40;
	//Bounding box at width
	this -> boundingBox.w = 150;
	//Bounding box at height
	this -> boundingBox.h = 200;
}
