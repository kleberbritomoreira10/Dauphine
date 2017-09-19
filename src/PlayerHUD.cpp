/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @PlayerHUD.cpp
* The settings of the player.
* Class that controll the initial configutations of the player.
* License: Copyright (C) 2014 Alke Games.
*/

#include "PlayerHUD.h"
#include "Game.h"
#include "Util.h"
#include "Logger.h"

/**
* The constructor.
* Initializes the attributes.
* @param player_ : The player of the game.
* @param potionsLeft : The potions in left side.
*/
PlayerHUD::PlayerHUD( Player *const player_ ):
	player( player_ ),
	potionsLeft( new Text( 200.0, 25.0, "res/fonts/maturasc.ttf", 50, "Potions: x" ) )
{
	for( unsigned int i = 0; i < TOTAL_HUD; i++ )
	{
		this -> playerHudSprites[ i ] = nullptr;
	}

	initializeSprites();

	for( int i = 0; i < TOTAL_HUD; i++ )
	{
		this -> canRenderHud[ i ] = true;
	}

}

/**
* The destructor.
* Deletes the instance.
*/
PlayerHUD::~PlayerHUD()
{
	// Deleting the potions in left.
	if( this -> potionsLeft != nullptr)
	{
		delete this -> potionsLeft;
		this -> potionsLeft = nullptr;
	}
}

/**
* Controll tthe uodates of the player.
*/
void PlayerHUD::update()
{
	// Verifying if the the player is not null.	
	if( this -> player != nullptr )
	{
		switch( this -> player -> life)
		{
			case 2:
				this -> canRenderHud[ 3 ] = false;
				break;
			case 1:
				this -> canRenderHud[ 2 ] = false;
				break;
			case 0:
				this -> canRenderHud[ 1 ] = false;
				break;
			default :
				break;
		}

		this -> potionsLeft -> changeText( ("Potions: "+ Util::toString( this -> player -> potionsLeft )).c_str() );
	}
}

/**
* Controlls the renderization of the states of player.
*/
void PlayerHUD::render()
{
	for( int i = 0; i < TOTAL_HUD; i++)
	{
		// Verifying if the element of the vector is valid.	
		if( this -> canRenderHud[ i ])
		{
			this -> playerHudSprites[ i ] -> render( 0, 0 );
		}
	}

	// Verifying if the potions of the left are not null.
	if( this -> potionsLeft != nullptr)
	{
		this -> potionsLeft -> render( 0, 0 );
	}

	// Verifying if the potions of the left are null.	
	else 
	{
		Log( WARN ) << "Potions left HUD text is null";
	}
}

/**
* Controlls the initialization of the sprites.
*/
void PlayerHUD::initializeSprites()
{
	this -> playerHudSprites[ 0 ] = Game::instance().getResources().get("res/images/hud/health_0.png");
	this -> playerHudSprites[ 1 ] = Game::instance().getResources().get("res/images/hud/health_33.png");
	this -> playerHudSprites[ 2 ] = Game::instance().getResources().get("res/images/hud/health_66.png");
	this -> playerHudSprites[ 3 ] = Game::instance().getResources().get("res/images/hud/health_99.png");
	this -> playerHudSprites[ 4 ] = Game::instance().getResources().get("res/images/hud/hud_no_health.png");	
}