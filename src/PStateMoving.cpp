/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @PStateMoving.cpp
 * File responsible for implementing movement, animation and soundtrack in specific parts of the game
 * License: Copyright (C) 2014 Alke Games.
 */

#include "PStateMoving.h"
#include "Logger.h"
#include "Game.h"

#define THROW_STRENGTH 30
#define THROW_DISTANCE 400
#define PLAYER_STATE_MOVING_BOX_X 58
#define PLAYER_STATE_MOVING_BOX_Y 72
#define PLAYER_STATE_MOVING_BOX_W 130
#define PLAYER_STATE_MOVING_BOX_H 160

/*
 * The state when the player is grounded and moving.
 */
void PStateMoving::enter()
{
  this -> box.x = PLAYER_STATE_MOVING_BOX_X;
  this -> box.y = PLAYER_STATE_MOVING_BOX_Y;
  this -> box.w = PLAYER_STATE_MOVING_BOX_W;
  this -> box.h = PLAYER_STATE_MOVING_BOX_H;
  this -> player -> getAnimation()->changeAnimation(4,2,9,false,0.7);

  Game::instance().get_audio_handler().addSoundEffect("res/audio/FX_NADINE/RUNNING_NADINE_01.wav");
}

/*
 * @see StatePlayer::exit
 */
void PStateMoving::exit()
{

}

/*
 * Method used to handle state moving
 * @param keyStates : receives an array containing the move 
 * @see StatePlayer::handleInput
 */
void PStateMoving::handleInput( const std::array<bool, GameKeys::MAX> keyStates_)
{
  // Aerial
  if ( !this -> player -> isGrounded)
  {
    this -> player->changeState(Player::player_states::AERIAL);
    return;     
  } else {
		Log ( INFO ) << "\tPlayer is not in ground";
	}

  // Jump
  if ( keyStates_[GameKeys::SPACE] && this -> player -> isGrounded )
  {
    this -> player -> jump();
    this -> player -> isGrounded = false;
    return;
  } else {
		Log ( INFO ) << "\tPlayer is not jumpping";
	}
  
  //Use potion
  if ( keyStates_[GameKeys::ACTION] )
  {
    // Game::instance().get_audio_handler().addSoundEffect("res/audio/FX_NADINE/WOOSH_NADINE_03.wav");
    this -> player->usePotion(THROW_STRENGTH, THROW_DISTANCE);
    return;
  } else {
		Log ( INFO ) << "\tPotion not used";
	}
  
  //Change state player
  if ( keyStates_[GameKeys::LATTACK])
  {
    this -> player -> changeState(Player::player_states::ATTACKMOVING);
    return;
  } else {
		Log ( INFO ) << "\tState not changed";
	}

  this -> player -> move(keyStates_[GameKeys::LEFT], keyStates_[GameKeys::RIGHT]);
  
  //Verify if roll player
  if ( keyStates_[GameKeys::ROLL])
  {
    this -> player->changeState(Player::player_states::ROLLING);
    return;
  } else {
		// No Action.
	}

  // Idle
  if ( this -> player -> velocity_x_axis < 1.0 && this -> player -> velocity_x_axis > (-1.0))
  {
    this -> player->changeState(Player::player_states::IDLE);
    return;
  } else {
		Log ( INFO ) << "\tState not in idle";
	}

  // Attack
  if ( keyStates_[GameKeys::LATTACK])
  {
    this -> player->changeState(Player::player_states::ATTACK);
    return;
  } else {
		Log ( INFO ) << "\tState not attacking";
	}
}

/*
 * The constructor.
 * @param player_ : Reference to the player.
 */
PStateMoving::PStateMoving( Player* const player_) :
    StatePlayer(player_)
{

}
