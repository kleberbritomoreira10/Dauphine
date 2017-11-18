/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PStateAttack.cpp
* The state when the player is grounded and moving.*/

#include "PStateAttack.h"
#include "Logger.h"
#include "Game.h"
#include <assert.h>
#include <cstddef>

#define TOTAL_TIME 0.466
#define NUMBER_OF_IMAGE 14
#define ANIMATION_POSITION_X 1
#define ANIMATION_POSITION_Y 7

/**
* @see StatePlayer::enter
*/
void PStateAttack::enter ()
{
  this -> box.x = 58;
  this -> box.y = 75;
  this -> box.w = 140;
  this -> box.h = 160;
  this -> player -> getAnimation() -> changeAnimation( ANIMATION_POSITION_X, ANIMATION_POSITION_Y, NUMBER_OF_IMAGE, false, TOTAL_TIME );

  Game::instance().get_audio_handler().addSoundEffect( "res/audio/FX_NADINE/WOOSH_NADINE_02.wav" );
}

/**
* @see StatePlayer::exit
*/
void PStateAttack::exit()
{
  this -> player -> can_attack = true;
}

/**
* @see StatePlayer::handleInput
*/
void PStateAttack::handleInput ( const std::array<bool, GameKeys::MAX> keyStates_ )
{
	( ( void )keyStates_ ); // Unused.
  
  if ( this -> player -> getAnimation() -> getCurrentFrame() == NUMBER_OF_IMAGE )
  {
    this -> player -> changeState( Player::player_states::IDLE );
  } else
    {
      //Nothing to do
    }
}

/**
* The constructor.
* @param player_ : Reference to the player.
*/
PStateAttack::PStateAttack( Player* const player_ ) : StatePlayer( player_ )
{

}
