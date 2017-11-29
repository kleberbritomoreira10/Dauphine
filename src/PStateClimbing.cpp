/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateClimbing.cpp
* The state of the climbing of the player.
* Class that controll the climbing of the player.
* License: Copyright (C) 2014 Alke Games.
*/

#include "PStateClimbing.h"
#include "Logger.h"
#include <assert.h>

#define POSITION_X 0
#define POSITION_Y 6
#define NUMBER_OF_IMAGES_ANIMATION 4
#define TOTAL_TIME_ANIMATION 1
#define BOX_POSITION_X 58
#define BOX_POSITION_Y 72
#define BOX_WIDTH 130
#define BOX_HEIGHT 145


/**
* Shows the animation of the player entering in climbing state.
*/
void PStateClimbing::enter()
{
	Log( DEBUG ) << "STATE CLIMBING";

	assert( this -> player != nullptr );

	this -> player -> isClimbing = true;

   	this -> box.x = BOX_POSITION_X;
    this -> box.y = BOX_POSITION_Y;
    this -> box.w = BOX_WIDTH;
    this -> box.h = BOX_HEIGHT;

	// Changing the current state animation to state climbing animation.
    this -> player -> getAnimation() -> changeAnimation( POSITION_X, POSITION_Y, NUMBER_OF_IMAGES_ANIMATION, 
														false, TOTAL_TIME_ANIMATION );

    this -> player -> velocity_y_axis = 0;

	handle_velocity_x_axis();
}

// Handles the player velocity in the X axis.
void PStateClimbing::handle_velocity_x_axis()
{
	if( !this -> player -> is_right )
	{
		this -> player -> velocity_x_axis = -0.001;

	}else
	{
		this -> player -> velocity_x_axis = 0.0;
	}
}

/**
* Ends with climbing state of the player.
*/
void PStateClimbing::exit()
{
	this -> player -> isClimbing = false; // Exits the climbing state.
	is_moving = false;
}

/**
* Handles the input keys.
* @param KeyStates_: Array with all the key states.
*/
void PStateClimbing::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	assert( this -> player != nullptr );

	this -> player -> moveVertical( keyStates_[GameKeys::UP], keyStates_[ GameKeys::DOWN ]); // Moves player DOWN and UP

	handle_climbing_animation();

	// Jump
	if( keyStates_[ GameKeys::SPACE ] )
	{
		Log( DEBUG ) << "Handling SPACE GameKey...";
		this -> player -> velocity_y_axis = -700;

		if( this -> player -> is_right)
		{
			this -> player -> velocity_x_axis = -500;

		}else
		{
			this -> player -> velocity_x_axis = 500;
		}

		this -> player -> changeState( Player::player_states::AERIAL ); // Changes the state to AERIAL.
		return;

	}else if( !this -> player -> isClimbing )
	{
		this -> player -> velocity_y_axis = -1000;
		this -> player -> changeState( Player::player_states::AERIAL ); // Changes the state to AERIAL.
		return;

	}else
	{
		// No action.
	}
}

// Handle animation when player is climbing.
void PStateClimbing::handle_climbing_animation()
{

	if( abs( this -> player -> velocity_y_axis ) < 1 )
	{
		// Change the animation to climbing animation when player is not moving.
		this -> player -> getAnimation() -> changeAnimation( this -> player -> getAnimation() -> getCurrentFrame() - 1,
			POSITION_Y, 1, false, 0 );
			is_moving = true;

	}else	if( is_moving )
				{
					// Change the animation to climbing animation when player is moving.
					this -> player -> getAnimation() -> changeAnimation( POSITION_X, POSITION_Y, NUMBER_OF_IMAGES_ANIMATION, 
																		false, TOTAL_TIME_ANIMATION );
					is_moving = false;

				}else
				{
					Log( DEBUG ) << "Invalid movement.";
				}
}

/**
* The constructor.
* Initializes the attributes.
*/
PStateClimbing::PStateClimbing( Player *const player_ ) :
	StatePlayer( player_ )
{
	assert( player_ );
}
