/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Boss.cpp
 * File responsible for implementing the boss, the most powerful enemy of the game. 
 * In this file the boss's characteristics are created and INITIALIZED, updated and destroyed when requested.
 * License: Copyright (C) 2014 Alke Games.
 */

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

double time_passsed = 0;

/*
 * Method used to create all characteristics Boss
 * @param x_ : Position on the x axis of the boss
 * @param y_ : Position on the y axis of the boss
 * @param PATH : The path to the desired sprite
 * @param player_ : Pointer that points to the type player.
 * @param SDL_FLIP_NONE : Renderer flip
 */
Boss::Boss( const double x_, const double y_, const std::string& PATH, Player* const player_ ) :
	DynamicEntity(x_, y_, PATH), potions_left(3), saw_player(false), potions(), life(8), has_shield(false), can_walk(true), player(player_), power_animation(nullptr), power_X(0.0), power_Y(0.0), power_is_activated(false), power(nullptr),
	  power_clip{0,0,0,0}, power_flip(SDL_FLIP_NONE), shield_animation(nullptr), shield(nullptr), shield_clip{0,0,0,0},
	  current_state(nullptr), animation(nullptr), states_map(), dead(false)
{
	initialize_states();

	// Initialize all the states for the Boss.
	this -> is_right = true;
	this -> speed = 400.0;
	this -> width = 360;
	this -> height = 360;
	this -> animation = new Animation(0, 0, this -> width, this -> height, 7, false);
	this -> power_animation = new Animation(0, 0, 0, 0, 0, false);
	this -> shield_animation = new Animation(0, 0, 340, 340, 6, false);
	this -> shield = Game::instance().get_resources().get("res/images/shield.png");
	this -> shield_animation->change_animation(0,0,3,false,1);
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

	destroy_states();
}

/*
 * Update the characteristics Boss
 * @param DELTA_TIME : Delta time (catch variation time).
 */
void Boss::update( const double DELTA_TIME)
{
	
	time_passsed += DELTA_TIME;

	scout_position(DELTA_TIME);
  
  //Characteristics boss to update
	this -> animation -> update( this -> animation_clip, DELTA_TIME);
	this -> power_animation -> update( this -> power_clip, DELTA_TIME);
	this -> shield_animation -> update( this -> shield_clip, DELTA_TIME);

	update_bounding_box();
  
  //Array to detect collision
	const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detect_collision();
	handle_collision( detections );

	update_position( DELTA_TIME);

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
 * @param camera_position_x : Position on the x-axis of the camera.
 * @param camera_position_y : Position on the y-axis of the camera.
 */
void Boss::render( const double camera_position_x, const double camera_position_y)
{
	const double dx = this -> x - camera_position_x;
	const double dy = this -> y - camera_position_y;
	
	if ( this -> sprite != nullptr )
	{
		// Sprite render.
		SDL_RendererFlip flip = get_flip();

		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> sprite -> render( dx - 120, dy, &this -> animation_clip, false, 0.0, nullptr, flip );
		} else {
			  this -> sprite -> render( dx, dy, &this->animation_clip, false, 0.0, nullptr, flip );
		  }
	}
	// Shield render.	
	if ( this -> has_shield )
	{
		SDL_RendererFlip flip = get_flip();
		if ( flip == SDL_FLIP_HORIZONTAL )
		{
			this -> shield->render( dx, dy, &this -> shield_clip );
		} else {
			  this -> shield->render(dx -120, dy, &this -> shield_clip);
		  }
	}

	//Constants for define position x e y to camera
	const double pdx = this -> power_X - camera_position_x;
	const double pdy = this -> power_Y - camera_position_y;

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
    potion -> render( camera_position_x, camera_position_y);
  }
}

/*
 * Initialize all the states in Boss.
 */
void Boss::initialize_states()
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
void Boss::destroy_states()
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
void Boss::change_state( const BStates state_)
{ 
	this -> current_state -> exit();
	this -> current_state = this -> states_map.at(state_);
	this -> current_state -> enter();
}

/*
 * Method for handling the type of collision.
 * @param detections_ : array to detect collisions.
 */
void Boss::handle_collision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_)
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
		this -> next_position_x = this -> x;
		this -> velocity_x_axis = 0.0;
	}
	//Check collision occurrence on right
	if ( detections_.at(CollisionSide::SOLID_RIGHT) )
	{
		this -> next_position_x = this -> x;
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
 * @param is_dead_ : boolean variable to check the boss's condition. 
 */
void Boss::set_dead(bool is_dead_)
{
	this -> dead = is_dead_;
}

/*
 * Check if Boss is alive
 */
bool Boss::is_dead()
{ 
	return this -> dead;
}

/*
 * Updating boundaries for the boss
 */
void Boss::update_bounding_box()
{ 
	this -> boundingBox.x = (int) this -> x + 40;
	this -> boundingBox.y = (int) this -> y + 40;
	this -> boundingBox.w = 150;
	this -> boundingBox.h = 200;
}
