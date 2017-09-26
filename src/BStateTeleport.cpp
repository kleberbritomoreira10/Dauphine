/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateTeleport.cpp
* Objects in this class control the behavior of teleportation to the boss
* License: Copyright (C) 2014 Alke Games.
*/

#include "BStateTeleport.h"
#include "Game.h"
#include "Collision.h"
#include "Logger.h"

double tptime = 0; // Teleport time that tetermines the behavior of the boss.
double pX = 0; // X-axis position.
double pY = 0; // Y-axis position.
int powerCollisionWidth = 0;
int powerCollisionHeight = 0;
int offset = 33;
bool right;
int direction = 0;

/*
 Method called on load.
 @see StateBoss::enter
 */
void BStateTeleport::enter ()
{
	// Log(DEBUG) << "STATE TELEPORT BOSS";
	this -> boss -> power = Game::instance (). get_resources (). get(
		"res/images/laser_sheet.png" );

	this -> boss -> power_animation -> changeWidthHeight ( 700, 340 );
	this -> boss -> power_animation -> changeAnimation ( 0, 0, 3, false, 0.5 );
	this -> boss -> player -> is_vulnerable = true;

}

/*
* Method called on unload.
* @see StateBoss::exit
*/
void BStateTeleport::exit ()
{
	this -> boss -> power_is_activated = false;
	this -> boss -> player -> is_vulnerable = true;
	this -> boss -> power_animation -> changeAnimation( 0, 0, 1, false, 0 );
	powerCollisionWidth = 0;
	powerCollisionHeight = 0;
	tptime = 0.0;
}

/**
* Update method for the state.
* While the Boss is on the state, this method runs every update.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void BStateTeleport::update ( const double DELTA_TIME )
{
	tptime += DELTA_TIME;

	if ( tptime < 3 )
	{
		this -> boss -> velocity_x_axis = 0;
		this -> boss -> velocity_y_axis = 0;

	} else if ( tptime >= 3 && tptime <= 3.05 )
	{

		this -> boss -> getAnimation() -> changeAnimation ( 0, 0, 1, false, 0 );
		this -> boss -> velocity_x_axis = 0;

		if ( this -> boss -> player -> is_right )
		{

			this -> boss -> x = this -> boss -> player -> x - 150 - 50;
			this -> boss -> y = this -> boss -> player -> y;

		}else
		{
			this -> boss -> x = this -> boss->player -> x + 150 + 50;
			this -> boss -> y = this -> boss -> player -> y;
		}

		this -> boss -> is_right = this -> boss -> player -> is_right;

		pX = this -> boss -> x;
		pY = this -> boss -> y;

		if ( !this -> boss -> is_right )
		{

			this -> boss -> power_flip = SDL_FLIP_HORIZONTAL;
			right = false;

		}else
		{

			this -> boss -> power_flip = SDL_FLIP_NONE;
			right = false;

		}
	}else if ( tptime > 3.05 && tptime < 4 )
	{
		this -> boss -> x = pX;
		this -> boss -> y = pY;

		if ( right )
		{

			this -> boss -> is_right = false;
			direction = 0;

		}else
		{
			this -> boss -> is_right = true;
			direction = 1;
		}
	}

	if ( tptime >= 4 && tptime <= 5 )
	{

		if ( this -> boss -> power_animation -> getCurrentFrame () == 1 )
		{

			powerCollisionWidth = 101;
			powerCollisionHeight = 30;

		}else if ( this -> boss -> power_animation -> getCurrentFrame () == 2 )
		{

			powerCollisionWidth = 539;
			powerCollisionHeight = 117;

		}else
		{

			powerCollisionWidth = 665;
			powerCollisionHeight = 262;

		}

		if ( this -> boss -> player -> is_right )
		{

			this -> boss -> power_X = pX;
			this -> boss -> power_Y = pY;

		}else
		{

			this -> boss -> power_X = pX;
			this -> boss -> power_Y = pY;

		}if ( tptime >= 4.5 )
		{

			this -> boss -> power_animation -> changeAnimation ( 2, 0, 1, false, 0 );
			powerCollisionWidth = 665;
			powerCollisionHeight = 262;

		}

		this -> boss -> power_is_activated = true;

		if ( Collision::rects_collided ( this -> boss -> player -> get_bounding_box (),
			{ ( int ) this -> boss -> power_X - direction * 665, ( int ) this -> boss -> power_Y
				+ offset, direction * 665, 262 }) )
		{

			if ( this -> boss -> player -> is_vulnerable )
			{
				this -> boss -> player -> life--;
				this -> boss -> player -> is_vulnerable = false;
			}
		}

	}else if ( tptime > 5 )
	{
		this -> boss -> change_state ( Boss::BStates::IDLE );
	}
}

// @param BOSS : Reference to the Boss.
BStateTeleport::BStateTeleport ( Boss *const BOSS ) :
	StateBoss( BOSS )
{

}
