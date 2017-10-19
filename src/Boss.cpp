/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Boss.cpp
 * File responsible for implementing the boss, the most powerful enemy of the game.
 * In this file the boss's characteristics are created and INITIALIZED, updated and destroyed when requested.
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "Boss.h"
#include "Logger.h"
#include "Game.h"
#include <cmath>
#include "BStateIdle.h"
#include "BStateAttack.h"
#include "BStateShield.h"
#include "BStateTeleport.h"
#include "BStateIcePrision.h"
#include "BStateMagicProjectile.h"
#include "Window.h"

#define ADD_STATE_EMPLACE(stateEnum, stateClass) this -> states_map.emplace(stateEnum, new stateClass( this ))
#define ADD_STATE_INSERT(stateEnum, stateClass) this -> states_map.insert(std::make_pair<BStates, StateBoss*>(stateEnum, new stateClass( this )));

double timePasssed = 0;

/*
 * Method used to create all characteristics Boss
 * @param x_ : Position on the x axis of the boss
 * @param y_ : Position on the y axis of the boss
 * @param PATH : The path to the desired sprite
 * @param player_ : Pointer that points to the type player.
 * @param SDL_FLIP_NONE : Renderer flip
 */
Boss::Boss( const double x_, const double y_, const std::string& PATH, Player* const player_ ) :
	DynamicEntity(x_, y_, PATH), potions_left(3), saw_player(false), potions(), life(8), has_shield(false), can_walk(true), player(player_), power_animation(nullptr), power_X_axis(0.0), power_Y_axis(0.0), power_is_activated(false), power(nullptr),
	  power_clip{0,0,0,0}, power_flip(SDL_FLIP_NONE), shield_animation(nullptr), shield(nullptr), shield_clip{0,0,0,0},
	  current_state(nullptr), animation(nullptr), states_map(), dead(false)
{
	initializeStates();

	// Initialize all the states for the Boss.
	this -> is_right = true;
	this -> speed = 400.0;
	this -> width = 360;
	this -> height = 360;
	this -> animation = new Animation(0, 0, this -> width, this -> height, 7, false);
	this -> power_animation = new Animation(0, 0, 0, 0, 0, false);
	this -> shield_animation = new Animation(0, 0, 340, 340, 6, false);
	this -> shield = Game::instance().getResources().get("res/images/shield.png");
	this -> shield_animation->changeAnimation(0,0,3,false,1);
	this -> current_state = this -> states_map.at(IDLE);
	this -> current_state->enter();

	//Check if player is playing
	if ( this -> player == nullptr )
	{
		Log(WARN) << "Passing a null player to the Boss.";
	}
}

/*
 * Method to represent the Boss
 */
Boss::~Boss()
{
	//Delete animation if null
	if ( this -> animation != nullptr )
	{
		delete this -> animation;
		this -> animation = nullptr;
	}

	//Delete power_animation if null
	if ( this -> power_animation != nullptr )
	{
		delete this -> power_animation;
		this -> power_animation = nullptr;
	}

  //Delete shield_animation if null
	if ( this -> shield_animation != nullptr )
	{
		delete this -> shield_animation;
		this -> shield_animation = nullptr;
	}

	//Exit Boss if current_state is different of the null
	if ( this -> current_state != nullptr )
	{
		this -> current_state -> exit();
	}

	this -> player = nullptr;

	destroyStates();
}

/*
 * Update the characteristics Boss
 * @param DELTA_TIME : Delta time (catch variation time).
 */
void Boss::update( const double DELTA_TIME)
{

	assert(DELTA_TIME >= 0);
	timePasssed += DELTA_TIME;

	scoutPosition(DELTA_TIME);

  //Characteristics boss to update
	this -> animation -> update( this -> animationClip, DELTA_TIME);
	this -> power_animation -> update( this -> power_clip, DELTA_TIME);
	this -> shield_animation -> update( this -> shield_clip, DELTA_TIME);

	updateBoundingBox();

  //Array to detect collision
	const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detectCollision();
	handleCollision( detections );

	updatePosition( DELTA_TIME);

	//Update current_state Boss
	this -> current_state -> update( DELTA_TIME);

  for( auto potion : this -> potions )
  {
    if ( !potion -> activated )
    {
      // Delete potion.
    }
     potion -> update( DELTA_TIME);
  }
}

/*
 * Render the characteristics Boss
 * @param cameraX_ : Position on the x-axis of the camera.
 * @param cameraY_ : Position on the y-axis of the camera.
 */
void Boss::render( const double cameraX_, const double cameraY_)
{
	const double dx = this -> x - cameraX_;
	const double dy = this -> y - cameraY_;

	if ( this -> sprite != nullptr )
	{
		// Sprite render.
		SDL_RendererFlip flip = getFlip();

		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> sprite -> render( dx - 120, dy, &this -> animationClip, false, 0.0, nullptr, flip );
		} else {
			  this -> sprite -> render( dx, dy, &this->animationClip, false, 0.0, nullptr, flip );
		  }
	}
	// Shield render.
	if ( this -> has_shield )
	{
		SDL_RendererFlip flip = getFlip();
		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> shield->render( dx, dy, &this -> shield_clip );
		} else {
			  this -> shield->render(dx -120, dy, &this -> shield_clip);
		  }
	}

	//Constants for define position x e y to camera
	const double pdx = this -> power_X_axis - cameraX_;
	const double pdy = this -> power_Y_axis - cameraY_;

	//Power render
	if ( this -> power != nullptr && this -> power_is_activated )
	{
		if ( this -> power_flip == SDL_FLIP_HORIZONTAL )
		{
			this -> power->render(pdx - this -> power_clip.w, pdy, &this ->power_clip, false, 0.0, nullptr, this -> power_flip);
		} else {
			  this->power->render(pdx, pdy, &this->power_clip, false, 0.0, nullptr, this->power_flip);
		  }
	}

  for ( auto potion : this -> potions )
  {
    potion -> render( cameraX_, cameraY_);
  }
}

/*
 * Initialize all the states in Boss.
 */
void Boss::initializeStates()
{
	ADD_STATE_INSERT(IDLE,				BStateIdle);
	ADD_STATE_INSERT(ATTACK,			BStateAttack);
	ADD_STATE_INSERT(SHIELD,			BStateShield);
	ADD_STATE_INSERT(TELEPORT,			BStateTeleport);
	ADD_STATE_INSERT(ICEPRISION,		BStateIcePrision);
	ADD_STATE_INSERT(MAGICPROJECTILE,	BStateMagicProjectile);
}

/*
 * Delete all the states in Boss.
 */
void Boss::destroyStates()
{
	std::map<BStates, StateBoss*>::const_iterator it;
	for ( it = this -> states_map.begin(); it != this -> states_map.end(); it++)
	{
		delete it -> second;
	}
}

/*
 * Exchange current state to Boss.
 * @param state_ : constant to know the state of the boss.
 */
void Boss::changeState( const BStates state_)
{
	this -> current_state -> exit();
	this -> current_state = this -> states_map.at(state_);
	this -> current_state -> enter();
}

/*
 * Method for handling the type of collision.
 * @param detections_ : array to detect collisions.
 */
void Boss::handleCollision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_)
{
	//Check collision occurrence on top
	if ( detections_.at( CollisionSide::SOLID_TOP ) )
	{
		this -> velocity_y_axis = 0.0;
	}
	//Check collision occurrence on bottom
	if ( detections_.at(CollisionSide::SOLID_BOTTOM ) )
	{
		this -> nextY -= fmod( this -> nextY, 64.0) - 16.0;
		this -> velocity_y_axis = 0.0;
	}
	//Check collision occurrence on left
	if ( detections_.at(CollisionSide::SOLID_LEFT ))
	{
		this -> nextX = this -> x;
		this -> velocity_x_axis = 0.0;
	}
	//Check collision occurrence on right
	if ( detections_.at(CollisionSide::SOLID_RIGHT) )
	{
		this -> nextX = this -> x;
		this -> velocity_x_axis = -0.001;
	}
}

/*
 * Method to determine that Boss use a Potion
 * @param strength_ : Force with which the portion is thrown.
 * @param distance_ : Distance with which the portion is thrown.
 */
void Boss::usePotion( const int strength_, const int distance_)
{
	assert( strength_ >= 0 );
	assert( distance_ >= 0 );
  if ( this -> potions_left > 0)
  {
    this -> potions_left--;
    const double potionX = (( this -> is_right ) ? this -> boundingBox.x + this -> boundingBox.w : this->boundingBox.x);
    Potion* potion = new Potion(potionX , this -> y, "res/images/potion.png", strength_, this -> velocity_x_axis, distance_, this ->is_right);
    this -> potions.push_back( potion );
  }
}

/*
 * Reference the animation.
 */
Animation *Boss::getAnimation()
{
	return ( this -> animation );
}

/*
 * Verify condition to Boss (dead or alive)
 * @param isDead_ : boolean variable to check the boss's condition.
 */
void Boss::set_dead(bool isDead_)
{
	this -> dead = isDead_;
}

/*
 * Check if Boss is alive
 */
bool Boss::isDead()
{
	return this -> dead;
}

/*
 * Updating boundaries for the boss
 */
void Boss::updateBoundingBox()
{
	this -> boundingBox.x = (int) this -> x + 40;
	this -> boundingBox.y = (int) this -> y + 40;
	this -> boundingBox.w = 150;
	this -> boundingBox.h = 200;
}
