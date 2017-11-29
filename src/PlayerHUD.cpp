#include "PlayerHUD.h"
#include "Game.h"
#include "Util.h"
#include "Logger.h"
#include <assert.h>

#define TEXT_POSITION_X 200.0
#define TEXT_POSITION_Y 25.00
#define TEXT_SIZE 50
#define CAMERA_POSITION_X 0
#define CAMERA_POSITION_Y 0

/**
* The constructor.
* Creates the HUD by setting the position and sprite.
* @param player_: Instance of the player.
*/
PlayerHUD::PlayerHUD( Player *const player_ ):

	player( player_ ),
	potions_left( new Text( TEXT_POSITION_X, TEXT_POSITION_Y, "res/fonts/maturasc.ttf", TEXT_SIZE, "Potions: x" ) )
{

	Log( INFO ) << "Loading PLayer HUD....";

	assert( player_ ); // Check if the player instance is not null.

	restart_hud_sprites();

	initializeSprites();

	restart_is_can_render_hud();

}

void PlayerHUD::restart_hud_sprites()
{
	for( unsigned int i = 0; i < TOTAL_HUD; i++ )
	{
		this -> player_hud_sprites[ i ] = nullptr; // Set null to all the player hud sprites.
	}
}

void PlayerHUD::restart_is_can_render_hud()
{
	for( int i = 0; i < TOTAL_HUD; i++ )
	{
		this -> is_can_render_hud[ i ] = true; // Set true to is_can_render_hud.
	}
}

/**
* The destructor.
* Exits the current state and destroys all states.
*/
PlayerHUD::~PlayerHUD()
{
	if( this -> potions_left != nullptr)
	{

		delete this -> potions_left; // Delete all the potions left.
		this -> potions_left = nullptr;

	}else
	{
		Log( WARN ) << "Potions left HUD text is null";
	}

	assert( potions_left == nullptr );
}

/**
* Updates the HUD.
* @see Player::updateInput, Player::updatePosition
*/
void PlayerHUD::update()
{
	if( this -> player != nullptr )
	{
		switch( this -> player -> life )
		{
			// Set the false to the is_can_render_hud.
			case 2:
				this -> is_can_render_hud[ 3 ] = false;
				break;
			case 1:
				this -> is_can_render_hud[ 2 ] = false;
				break;
			case 0:
				this -> is_can_render_hud[ 1 ] = false;
				break;
			default :
				// No action.
				break;

		}
	}else
		{
			Log( WARN ) << "Player is NULL!";
		}

		// Change the potions left HUD text.
		this -> potions_left -> changeText( ( "Potions: " + Util::toString( this -> player -> potions_left ) ).c_str() );

}

/**
* Renders the HUD.
* Uses the HUD's sprite render method.
* @see Sprite::render
*/
void PlayerHUD::render()
{
	render_hud_sprites();

	render_potions_left();
}

// Render all the hud sprites.
void PlayerHUD::render_hud_sprites()
{
	for( int i = 0; i < TOTAL_HUD; i++)
	{
		if( this -> is_can_render_hud[ i ])
		{
			this -> player_hud_sprites[ i ] -> render( CAMERA_POSITION_X, CAMERA_POSITION_Y ); // Render the hud sprites.

		}else
		{
			Log( DEBUG ) << "HUD cannot be rendered.";
		}
	}
}

// Render all the player's potions left.
void PlayerHUD::render_potions_left()
{
	if( this -> potions_left != nullptr )
	{
		this -> potions_left -> render( CAMERA_POSITION_X, CAMERA_POSITION_Y ); // Render all the player's potions left.

	}else
	{
		Log( WARN ) << "Potions left HUD text is null";
	}
}

void PlayerHUD::initializeSprites()
{
	this -> player_hud_sprites[ 0 ] = Game::instance().getResources().get( "res/images/hud/health_0.png" );
	this -> player_hud_sprites[ 1 ] = Game::instance().getResources().get( "res/images/hud/health_33.png" );
	this -> player_hud_sprites[ 2 ] = Game::instance().getResources().get( "res/images/hud/health_66.png" );
	this -> player_hud_sprites[ 3 ] = Game::instance().getResources().get( "res/images/hud/health_99.png" );
	this -> player_hud_sprites[ 4 ] = Game::instance().getResources().get( "res/images/hud/hud_no_health.png" );

	assert( this -> player_hud_sprites[ 0 ] != nullptr );
	assert( this -> player_hud_sprites[ 1 ] != nullptr );
	assert( this -> player_hud_sprites[ 2 ] != nullptr );
	assert( this -> player_hud_sprites[ 3 ] != nullptr );
	assert( this -> player_hud_sprites[ 4 ] != nullptr );
}
