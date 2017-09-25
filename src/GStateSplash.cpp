/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @AudioHandler.cpp
* The audio handler.
* The state for the initial splash screen.
* Game state that will contain the initial splash images, 
before the main menu state is called.
* License: Copyright (C) 2014 Alke Games.*/

#include "GStateSplash.h"
#include "LuaScript.h"
#include "Game.h"

#include <string>

/**
* The constructor.
* Initializes all the attributes.
*/
GStateSplash::GStateSplash() :
	currentSplash( 0 ),
	passedTime( 0.0 ),
	lifeTime( 0.0 ),
	ix( -300.0 )
{

	// Setting null for all the splash images.
	for ( unsigned int i = 0; i < SplashImages::TOTAL_SPLASH_IMAGES; i++ )
	{
		this -> images[i] = nullptr;
	}
}

/**
* The destructor.
*/
GStateSplash::~GStateSplash()
{

}

/**
* Loads the level.
* From the Splash.lua script, loads all the necessary objects.
*/
void GStateSplash::load()
{
	Log( DEBUG ) << "Loading splash screens...";

	// Loading all the lua splash images.
	LuaScript luaSplash( "lua/Splash.lua" );
	const std::string pathLogo = luaSplash.unlua_get<std::string>( "splash.images.alke" );
	const std::string pathTechs = luaSplash.unlua_get<std::string>( "splash.images.techs" );
	const std::string pathLicenses = luaSplash.unlua_get<std::string>( "splash.images.licenses" );
	const std::string pathEsrb = luaSplash.unlua_get<std::string>( "splash.images.esrb" );
	const double luaLifeTime = luaSplash.unlua_get<double>( "splash.lifeTime" );

	// Loading splash images resources.
	this -> images[SplashImages::ALKE_LOGO] = Game::instance().getResources().get( pathLogo );
	this -> images[SplashImages::TECHS] = Game::instance().getResources().get( pathTechs );
	this -> images[SplashImages::LICENSES] = Game::instance().getResources().get( pathLicenses );
	this -> images[SplashImages::ESRB] = Game::instance().getResources().get( pathEsrb );

	this -> lifeTime = luaLifeTime;
}

/**
* Unloads everything that was loaded.
* @see GStateSplash::load
*/
void GStateSplash::unload()
{
	Log( DEBUG ) << "\tUnloading splash screens...";
	this -> currentSplash = 0;
	cleanEntities();
}

/**
* Updates the objects within the StateGame.
* @param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void GStateSplash::update( const double dt_ )
{
	this -> passedTime += dt_;

	Game::instance().getFade().fadeOut( 0, 0.002 );

	// Increment current image x position.
	if ( this -> ix < 0.0 )
	{
		this -> ix += 5.0;
	}

	// Updating splash images.
	if ( this -> passedTime >= this -> lifeTime )
	{
		if ( this -> currentSplash >= SplashImages::TOTAL_SPLASH_IMAGES - 1 )
		{
			Game::instance().setState( Game::GStates::MENU );
		} else
		{
			Game::instance().getFade().fadeIn( 100, 0.002 );
			this -> passedTime = 0.0;
			this -> ix = -300;
			this -> currentSplash++;
		}
	}

	// Check if SPACE was pressed, to skip the splash images.
	std::array< bool, GameKeys::MAX > keyStates = Game::instance().getInput();
	if ( keyStates[GameKeys::SPACE] == true )
	{
		Game::instance().setState( Game::GStates::MENU );
		return;
	}

}

/**
* Renders the state.
* Always renders on 0,0 position.
* @see Sprite::render
*/
void GStateSplash::render()
{
	// Rendering current splash.
	if ( this -> images[ this -> currentSplash] != nullptr )
	{
		this -> images[ this -> currentSplash ] -> render( this -> ix, 0, nullptr, true );
	} else
	{
		Log( WARN ) << "No image set for the splash screen!";
	}
}
