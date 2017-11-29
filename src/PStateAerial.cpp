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
#include <assert.h>

#define POSITION_X 4
#define POSITION_Y 3
#define NUMBER_OF_IMAGES_ANIMATION 14
#define TOTAL_TIME_ANIMATION 1.4

/**
* Shows the animation of the player entering in aerial state.
*/
void PStateAerial::enter()
{
  Log( INFO ) << "Entering the aerial state...";

  this -> box.x = 58;
  this -> box.y = 72;
  this -> box.w = 130;
  this -> box.h = 145;

  // Change the player animation to the state aerial animation.
	this -> player -> getAnimation() -> changeAnimation( POSITION_X, POSITION_Y, NUMBER_OF_IMAGES_ANIMATION, 
                                                      false, TOTAL_TIME_ANIMATION );

	this -> player -> isGrounded = false;

    // Adding state aerial sound effect.
    Game::instance().get_audio_handler().addSoundEffect( "res/audio/FX_NADINE/FALL_NADINE_01.wav" );
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
void PStateAerial::handleInput( const std::array< bool, GameKeys::MAX > keyStates_ )
{

	// Idle
  if( this -> player -> isGrounded)
  {
    this -> player -> changeState( Player::player_states::IDLE ); // Changing state to idle.
    return;

  }else if( keyStates_ [ GameKeys::LATTACK ])
  {
    this -> player -> changeState( Player::player_states::ATTACKJUMPING ); // Changing state to Attack Jumping.
    return;
  }else
  {
    // Gravity
    this -> player -> applyGravity();
    // Move (while on air)
    this -> player -> move( keyStates_[ GameKeys::LEFT ], keyStates_[ GameKeys::RIGHT ]);
  }
}

/**
* The constructor.
* Initializes the attributes.
*/
PStateAerial::PStateAerial( Player *const player_ ) :
	StatePlayer( player_ )
{
  assert( player_ );
}
