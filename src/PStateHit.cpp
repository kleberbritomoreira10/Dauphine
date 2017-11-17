/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @PStateHit.cpp
 * File responsible for checking the state of the beat, manipulating the impact of the beat.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "PStateHit.h"
#include "Logger.h"

#define PLAYER_STATE_MOVING_BOX_X 58
#define PLAYER_STATE_MOVING_BOX_Y 72
#define PLAYER_STATE_MOVING_BOX_W 130
#define PLAYER_STATE_MOVING_BOX_H 160
#define PLAYER_VELOCITY_X 5000
#define PLAYER_VELOCITY_Y -130
#define ZERO 0

/*
 * Method to check the position of the beat
 * @see StatePlayer::enter
 */
void PStateHit::enter()
{
  //Displays this log on the terminal
  Log(DEBUG) << "STATE ATTACK JUMPING";

  this -> box.x = PLAYER_STATE_MOVING_BOX_X;
  this -> box.y = PLAYER_STATE_MOVING_BOX_Y;
  this -> box.w = PLAYER_STATE_MOVING_BOX_W;
  this -> box.h = PLAYER_STATE_MOVING_BOX_H;
  this -> player -> getAnimation() -> changeAnimation(4, 8, 1, false, ZERO);

  int direction = ZERO;
  if ( this -> player -> is_right )
  {
    direction = -1;
  } else {
    direction = 1;
  }
  this -> player -> velocity_y_axis = -PLAYER_VELOCITY_Y;
  this -> player -> velocity_x_axis = PLAYER_VELOCITY_X * direction;
}

/*
 * Verify the direction
 * @see StatePlayer::exit
 */
void PStateHit::exit()
{
  if ( this -> player -> is_right )
  {
    this -> player -> velocity_x_axis = 0.0;
  } else {
    this -> player -> velocity_x_axis = -0.001;
  }
}

/*
 * @see StatePlayer::handleInput
 */
void PStateHit::handleInput( const std::array<bool, GameKeys::MAX> keyStates_)
{
  ((void) keyStates_); // Unused.
}

/*
 * The constructor.
 * @param player_ : Reference to the player.
 */
PStateHit::PStateHit( Player* const player_) : StatePlayer(player_)
{

}
