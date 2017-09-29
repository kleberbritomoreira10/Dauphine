/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @DynamicEntity.cpp
 * Subclass of Entity.
 * The objects of this class are entities, but have physics applied to them (such as velocity).
 * License: Copyright (C) 2014 Alke Games.
 */

#include "DynamicEntity.h"
#include "Logger.h"
#include "Collision.h"
#include "Math.h"

/*
* @param x_ : position in x axis.
* @param y_ : position in y axis.
*/
DynamicEntity::DynamicEntity ( const double x_, const double y_, const std::string &PATH ) :

	Entity ( x_, y_, PATH ),
	velocity_x_axis ( 0.0 ),
	velocity_y_axis ( 0.0 ),
	speed ( 20.0 ),
	maxSpeed ( 550.0 ),
	isGrounded ( false ),
	next_position_x ( x_ ),
	nextY ( y_ ),
	reached_level_end ( false ),
    activated ( false ),
    strength ( 0 ),
    distance ( 0 ),
    flightTime ( 0.0 ),
    isClimbing ( false ),
    level_width ( 0 ),
	level_height ( 0 )

{

}

DynamicEntity::~DynamicEntity ()
{

}

// @param width_,height_ : Tells the player what the width and height of the level is.

void DynamicEntity::set_level_width_height ( const unsigned int width_, const unsigned int height_ )
{

	this -> level_width = width_;
	this -> level_height = height_;

}


/* Updates the position of the dynamic entity
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
* of processing speed.
*/
void DynamicEntity::update_position ( const double DELTA_TIME )
{

	( ( void ) DELTA_TIME ); // Unused?

	this -> x = this -> next_position_x;
	this -> y = this -> nextY;

	this -> is_right = ( this -> velocity_x_axis >= 0.0 );
}

/*
Update is based on what input was recieved, and the players velocity.
@param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
of processing speed.
*/
void DynamicEntity::scout_position ( const double DELTA_TIME )
{
	//
	this -> next_position_x += this -> velocity_x_axis * DELTA_TIME;
	this -> nextY += this -> velocity_y_axis * DELTA_TIME;

}

std::array < bool, CollisionSide::SOLID_TOTAL > DynamicEntity::detect_collision ()
{

	std::array < bool, CollisionSide::SOLID_TOTAL > detections;
	detections.fill ( false );

	for ( auto tileBox : this -> collisionRects )
	{

		const Collision::RectangleSide side = Collision::rectsCollidedSide (
			this -> boundingBox, tileBox.rect);

		if ( side != Collision::RectangleSide::NONE )
		{

			if ( tileBox.type == LEVEL_END )
			{
				this -> reached_level_end = true;
			}

			switch ( side )
			{

				case Collision::RectangleSide::TOP:

				 // Hitting under the tile.
					detections.at ( SOLID_TOP ) = ( tileBox.type != JUMP_THROUGH ) ? true : false;

					break;

				case Collision::RectangleSide::BOTTOM: // Hitting top of the tile.

					detections.at ( SOLID_BOTTOM ) = true;

					if( tileBox.type == JUMP_THROUGH )
					{

						// Going up, not colliding
						if ( this -> velocity_y_axis < 0 )
						{

							detections.at ( SOLID_BOTTOM ) = false;
							//Log(DEBUG) << "Up and go";

						}else
						{
							// Do nothing.
						}

						// Going down and goes through tile top
						if ( this -> velocity_y_axis >= 0.0 && ( this -> boundingBox.y
							+ this -> boundingBox.h ) > tileBox.rect.y + tileBox.rect.h )
						{

							detections.at ( SOLID_BOTTOM ) = false;
							//Log(DEBUG) << "Not so fast";

						}else
						{

							detections.at ( SOLID_BOTTOM ) = true;
							//Log(DEBUG) << "Stop on top";

						}
					}
					break;

				case Collision::RectangleSide::RIGHT: // Hitting right side of the tile.

					detections.at ( SOLID_RIGHT ) = ( tileBox.type != JUMP_THROUGH ) ? true : false;

					if ( tileBox.type == CLIMB && !this -> isGrounded )
					{

						// Log(DEBUG) << "CLIMB RIGHT";
						this -> isClimbing = true;

					}else
					{

						if ( this -> isClimbing )
						{

							// Log(DEBUG) << "EXIT CLIMB right";
							this -> isClimbing = false;

						}
					}

					break;

				case Collision::RectangleSide::LEFT: // Hitting left side of the tile.

					detections.at ( SOLID_LEFT ) = ( tileBox.type != JUMP_THROUGH ) ? true : false;

					if ( tileBox.type == CLIMB && !this -> isGrounded )
					{

						// Log(DEBUG) << "~~~~ CLIMB LEFT";
						this -> isClimbing = true;

					}else
					{

						if( this -> isClimbing )
						{

							// Log(DEBUG) << "EXIT CLIMB left";
							this -> isClimbing = false;

						}
					}

					break;

				default:
					Log ( ERROR ) << "Unknown rectangle side collided with a dynamic entity.";
					break;
			} // switch (side) -- process all collisions on rectangle
		}else{
			// No collision.
		}

	} // for collisionRects -- process the tileBox Rects in the rectangle

	return detections;
}

// Apply the jump on a dynamic entity speed on the y axis.
void DynamicEntity::jump ()
{
	this -> velocity_y_axis = ( -1 ) * 1210.0;
}

void DynamicEntity::applyGravity ()
{
	// Apply the gravity on a dynamic entity speed on the y axis.
	if ( this->velocity_y_axis + 50 < this->maxSpeed * 2 )
	{
		this -> velocity_y_axis += 50;
	}else{
		// Do nothing.
	}
}

void DynamicEntity::move ( const bool movingLeft_, const bool movingRight_ )
{

	const double turnHandle = 5.5;
	// Applies Apply right and left movement on entity speed on the x axis.
	if ( movingLeft_ || movingRight_ )
	{

		if ( movingLeft_ )
		{

			if ( this -> velocity_x_axis > 0.0)
			{

				this -> velocity_x_axis -= this -> speed * turnHandle;

			}else{

				this -> velocity_x_axis -= this -> speed;

			} // if -- left moving

			this -> velocity_x_axis = ( this -> velocity_x_axis < -this -> maxSpeed ) ? -this -> maxSpeed : this -> velocity_x_axis;

		}

		if ( movingRight_ )
		{

			if ( this -> velocity_x_axis < 0.0 )
			{

				this -> velocity_x_axis += this -> speed * turnHandle;

			}else
			{

				this -> velocity_x_axis += this -> speed;

			}
			this -> velocity_x_axis = ( this -> velocity_x_axis > this -> maxSpeed ) ? this -> maxSpeed : this -> velocity_x_axis;
		} // if -- right moving
	}else
	{
		slowVx ();
	}

}

void DynamicEntity::moveVertical ( const bool movingUp_, const bool movingDown_ )
{

	const double turnHandle = 5.5;
	// Apply the movement up and down on entity speed on the y axis
	if ( movingUp_ || movingDown_ )
	{

		if ( movingUp_ )
		{

			if ( this -> velocity_y_axis > 0.0 )
			{

				this -> velocity_y_axis -= this -> speed * turnHandle;

			}else
			{

				this -> velocity_y_axis -= this -> speed;

			}

			this -> velocity_y_axis = ( this -> velocity_y_axis < -this -> maxSpeed ) ? -this -> maxSpeed : this -> velocity_y_axis;

		} // if -- moving up

		if ( movingDown_ )
		{

			if ( this -> velocity_y_axis < 0.0 )
			{

				this -> velocity_y_axis += this -> speed * turnHandle;

			}else
			{

				this -> velocity_y_axis += this -> speed;

			}

			this -> velocity_y_axis = ( this -> velocity_y_axis > this -> maxSpeed ) ? this -> maxSpeed : this -> velocity_y_axis;
		} // // if -- moving down
	}else
	{
		slowVy ();
	}

} // if -- moving down and moving up

void DynamicEntity::slowVx ()
{
	// Decreases the dynamic entity speed on the x axis.
	const int vsign = Math::sign ( this -> velocity_x_axis );

	this -> velocity_x_axis -= 100 * vsign;

	if ( Math::sign ( this -> velocity_x_axis ) != vsign )
	{
        this -> velocity_x_axis = 0.0001 * vsign;
	}
}

void DynamicEntity::slowVy ()
{
	// Decreases the dynamic entity speed on the y axis.
	const int vsign = Math::sign ( this -> velocity_y_axis );

	this -> velocity_y_axis -= 1000 * vsign;

	if ( Math::sign ( this -> velocity_y_axis ) != vsign )
	{

        this -> velocity_y_axis = 0.0001 * vsign;

	}
}

void DynamicEntity::roll ()
{

	const double rollStrength = 120.0;

	// Apply the roll on a dynamic entity speed on the x axis.
	if ( this -> is_right )
	{
		this -> velocity_x_axis = rollStrength * this -> speed;
	}else
	{
		this -> velocity_x_axis = -rollStrength * this -> speed;
	}
}

void DynamicEntity::aim ( Crosshair *const crosshair, const double direction)
{
	// Apply the aim on a dynamic entity speed on the x axis.
	const double velocity = 10.0;
	crosshair -> x += velocity * direction;

}

SDL_RendererFlip DynamicEntity::get_flip ()
{

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if ( !this->is_right )
	{
		flip = SDL_FLIP_HORIZONTAL;
	}else{
		// Do nothing.
	}

	return flip;
}
