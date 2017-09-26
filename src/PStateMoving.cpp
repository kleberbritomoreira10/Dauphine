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

/*
 * The state when the player is grounded and moving.
 */
void PStateMoving::enter()
{
  this -> box.x = 58;
  this -> box.y = 72;
  this -> box.w = 130;
  this -> box.h = 160;
  this -> player -> getAnimation()->changeAnimation(4,2,9,false,0.7);

  Game::instance().getAudioHandler().addSoundEffect("res/audio/FX_NADINE/RUNNING_NADINE_01.wav");
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
    this -> player->changeState(Player::PStates::AERIAL);
    return;     
  }

  // Jump
  if ( keyStates_[GameKeys::SPACE] && this -> player -> isGrounded )
  {
    this -> player -> jump();
    this -> player -> isGrounded = false;
    return;
  }
  
  //Use potion
  if ( keyStates_[GameKeys::ACTION] )
  {
    // Game::instance().getAudioHandler().addSoundEffect("res/audio/FX_NADINE/WOOSH_NADINE_03.wav");
    this -> player->usePotion(THROW_STRENGTH, THROW_DISTANCE);
    return;
  }
  
  //Change state player
  if ( keyStates_[GameKeys::LATTACK])
  {
    this -> player -> changeState(Player::PStates::ATTACKMOVING);
    return;
  }

  this -> player -> move(keyStates_[GameKeys::LEFT], keyStates_[GameKeys::RIGHT]);
  
  //Verify if roll player
  if ( keyStates_[GameKeys::ROLL])
  {
    this -> player->changeState(Player::PStates::ROLLING);
    return;
  }

  // Idle
  if ( this -> player -> vx < 1.0 && this -> player -> vx > (-1.0))
  {
    this -> player->changeState(Player::PStates::IDLE);
    return;
  }

  // Attack
  if ( keyStates_[GameKeys::LATTACK])
  {
    this -> player->changeState(Player::PStates::ATTACK);
    return;
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
