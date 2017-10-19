/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @PStateHit.cpp
 * File responsible for checking the state of the beat, manipulating the impact of the beat.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "PStateHit.h"
#include "Logger.h"

/*
 * Method to check the position of the beat
 * @see StatePlayer::enter
 */
void PStateHit::enter()
{
  //Displays this log on the terminal
  Log(DEBUG) << "STATE ATTACK JUMPING";

  this -> box.x = 58;
  this -> box.y = 72;
  this -> box.w = 130;
  this -> box.h = 145;
  this -> player -> getAnimation() -> changeAnimation(4, 8, 1, false, 0);

  int direction = 0;
  if ( this -> player -> is_right )
  {
    direction = -1;
  } else {
    direction = 1;
  }
  this -> player -> velocity_y_axis = -130;
  this -> player -> velocity_x_axis = 5000 * direction;
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
  } else
	{
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
