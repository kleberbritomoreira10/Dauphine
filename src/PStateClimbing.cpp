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

bool isMoving = false;

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

	this -> player -> vy = 0;
	
	// Verifying if the player is in the right.
    if( !this -> player -> isRight )
    {
		this -> player -> vx = -0.001;
	}
	
	// Verifying if the player is not the in right.	
	else
	{
		this -> player -> vx = 0.0;
	}

}

/**
* Ends with climbing state of the player.
*/
void PStateClimbing::exit()
{
	this -> player -> isClimbing = false;
	isMoving = false;
}

void PStateClimbing::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{

	this -> player -> moveVertical( keyStates_[GameKeys::UP], keyStates_[ GameKeys::DOWN ]);

	// Verifying if the player is not in the right position.	
	if( abs( this -> player -> vy ) < 1 )
	{
		this -> player -> getAnimation() -> changeAnimation( this -> player -> getAnimation() -> getCurrentFrame() - 1,
			6, 1, false, 0 );
			isMoving = true;
	}
	else
	{
		// Verifying if the player is in movement.		
		if( isMoving )
		{
			this -> player -> getAnimation() -> changeAnimation( 0, 6, 4, false, 1 );
			isMoving = false;
		}
	}

	// Verifying if the keys of the game is equal a space and it is pressed.	
	if( keyStates_[ GameKeys::SPACE ] )
	{
		Log( DEBUG ) << "entrou";		
		this -> player -> vy = -700;

		// Verifying if the player is in the right.
		if( this -> player -> isRight)
		{
			this -> player -> vx = -500;
		}

		// Verifying if the player is not in the right.		
		else
		{
			this -> player -> vx = 500;
		}

		this -> player -> changeState( Player::PStates::AERIAL );
		return;
	}

	// Verifying if the player is climbing.	
	if( !this -> player -> isClimbing)
	{
		this -> player -> vy = -1000;
		this -> player -> changeState( Player::PStates::AERIAL );
		return;
	}
}

/**
* The constructor.
* Initializes the attributes.
* @param player_ : The player of the game.
*/
PStateClimbing::PStateClimbing( Player *const player_ ) :
	StatePlayer( player_ )
{

}
