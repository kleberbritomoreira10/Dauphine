#include "PStateRolling.h"
#include "Logger.h"
#include "Game.h"
#include <assert.h>
#include <cstddef>

void PStateRolling::enter ()
{
  this -> box.x = 58;
  this -> box.y = 72;
  this -> box.w = 140;
  this -> box.h = 160;
  this -> player -> getAnimation() -> changeAnimation( 6, 4, 9, true, 0.6 );
  this -> player -> roll();

  Game::instance().get_audio_handler().addSoundEffect( "res/audio/FX_NADINE/ROLLING_NADINE_01.wav" );
}

void PStateRolling::exit()
{

}

void PStateRolling::handleInput ( const std::array<bool, GameKeys::MAX> keyStates_ )
{
  // Aerial
  if ( !this -> player -> isGrounded )
  {
    this -> player -> changeState( Player::player_states::AERIAL );
    return;
  } else
    {
      //Nothing to do
    }

  this -> player -> slowVx();

  // Jump
  if ( keyStates_[ GameKeys::SPACE ] && this -> player -> isGrounded )
  {
    this -> player -> jump();
    this -> player -> changeState( Player::player_states::AERIAL );
    return;
  } else
    {
      //Nothing to do
    }

// Idle
  if ( this -> player -> velocity_x_axis < 1.0 && this -> player -> velocity_x_axis > ( -1.0 ) )
  {
    this -> player -> changeState( Player::player_states::IDLE );
    return;
  } else
    {
      //Nothing to do
    }
}

PStateRolling::PStateRolling ( Player* const player_ ) : StatePlayer( player_ )
{

}
