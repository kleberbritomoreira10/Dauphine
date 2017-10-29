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

bool is_moving = false;

/**
* Shows the animation of the player entering in climbing state.
*/
void PStateClimbing::enter()
{
	Log( DEBUG ) << "STATE CLIMBING";

	this -> player -> isClimbing = true;

   	this -> box.x = 58;
    this -> box.y = 72;
    this -> box.w = 130;
    this -> box.h = 145;

    this -> player -> getAnimation() -> changeAnimation( 0, 6, 4, false, 1 );

    this -> player -> velocity_y_axis = 0;

		handle_velocity_x_axis();
}

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
	this -> player -> isClimbing = false;
	is_moving = false;
}

void PStateClimbing::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{

	this -> player -> moveVertical( keyStates_[GameKeys::UP], keyStates_[ GameKeys::DOWN ]);

	handle_climbing_animation();

	// Jump
	if( keyStates_[ GameKeys::SPACE ] )
	{
		Log( DEBUG ) << "entrou";
		this -> player -> velocity_y_axis = -700;

		if( this -> player -> is_right)
		{
			this -> player -> velocity_x_axis = -500;

		}else
		{
			this -> player -> velocity_x_axis = 500;
		}

		this -> player -> changeState( Player::player_states::AERIAL );
		return;

	}else if( !this -> player -> isClimbing )
	{
		this -> player -> velocity_y_axis = -1000;
		this -> player -> changeState( Player::player_states::AERIAL );
		return;

	}else
	{
		// No action.
	}
}

void PStateClimbing::handle_climbing_animation()
{
	if( abs( this -> player -> velocity_y_axis ) < 1 )
	{
		this -> player -> getAnimation() -> changeAnimation( this -> player -> getAnimation() -> getCurrentFrame() - 1,
			6, 1, false, 0 );
			is_moving = true;

	}else	if( is_moving )
				{
					this -> player -> getAnimation() -> changeAnimation( 0, 6, 4, false, 1 );
					is_moving = false;

				}else
				{
					// No action.
				}
}

/**
* The constructor.
* Initializes the attributes.
*/
PStateClimbing::PStateClimbing( Player *const player_ ) :
	StatePlayer( player_ )
{

}
