/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateAttack.cpp
* The state when the player is grounded and moving.*/

#include "PStateAttack.h"
#include "Logger.h"
#include "Game.h"

/**
* @see StatePlayer::enter
*/
void PStateAttack::enter ()
{
  this -> box.x = 58;
  this -> box.y = 75;
  this -> box.w = 140;
  this -> box.h = 160;

  this -> player -> getAnimation() -> changeAnimation( 1, 7, 14, false, 0.466 );

  Game::instance().getAudioHandler().addSoundEffect( "res/audio/FX_NADINE/WOOSH_NADINE_02.wav" );
}

/**
* @see StatePlayer::exit
*/
void PStateAttack::exit()
{
  this -> player -> canAttack = true;
}

/**
* @see StatePlayer::handleInput
*/
void PStateAttack::handleInput ( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	( ( void )keyStates_ ); // Unused.
  
  if ( this -> player -> getAnimation() -> getCurrentFrame() == 14 )
  {
    this -> player -> changeState( Player::PStates::IDLE );
  }
}

/**
* The constructor.
* @param player_ : Reference to the player.
*/
PStateAttack::PStateAttack( Player* const player_ ) :
  StatePlayer( player_ )
{

}
