/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateAerial.cpp
* The state of the aerial postions of the player.
* Class that controll the aerial states of the player.
* License: Copyright (C) 2014 Alke Games.
*/

#include "PStateAerial.h"
#include "Logger.h"
#include "Game.h"

/**
* Shows the animation of the player entering in aerial state.
*/
void PStateAerial::enter()
{
  this -> box.x = 58;
  this -> box.y = 72;
  this -> box.w = 130;
  this -> box.h = 145;

	this -> player -> getAnimation() -> changeAnimation( 4, 3, 14, false, 1.4 );
	this -> player -> isGrounded = false;

    Game::instance().getAudioHandler().addSoundEffect("res/audio/FX_NADINE/FALL_NADINE_01.wav");
}

/**
* Ends with the aerial state of the player.
*/
void PStateAerial::exit()
{
  this -> player -> isClimbing = false;
}

/**
* Controlls the input for player entering in aerial mode.
* @param keyStates_: Array with frames of player positions.
*/
void PStateAerial::handleInput( const std::array<bool, GameKeys::MAX> keyStates_ )
{

	// Idle
  if( this -> player -> isGrounded)
  {
    this -> player -> changeState( Player::PStates::IDLE );
    return;
  }

  if( keyStates_[ GameKeys::LATTACK ])
  {
    this -> player -> changeState( Player::PStates::ATTACKJUMPING );
    return;
  }

	// Gravity
	this -> player -> applyGravity();

	// Move (while on air)
  this -> player -> move( keyStates_[ GameKeys::LEFT ], keyStates_[ GameKeys::RIGHT ]);
}

/**
* The constructor.
* Initializes the attributes.
*/
PStateAerial::PStateAerial( Player* const player_ ) :
	StatePlayer( player_ )
{

}
