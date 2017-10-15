/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FPSWrapper.cpp
 * Objects in this file are responsible for FPS manager and sets framerate.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "FPSWrapper.h"
#include "Logger.h"
#include "Configuration.h"

/**
* Initializes FPS managing.
* Initializes the FPS manager and sets it to the desired framerate
* 	(Configuration::maxFramerate).
* @param fps_manager_ : The FPS manager to handle.
* @note Will warn if failed to set FPS.
*/
void FPSWrapper::initialize ( FPSmanager& fps_manager_ )
{

	SDL_initFramerate ( &fps_manager_ );

	const int framerateIsSet = SDL_setFramerate ( &fps_manager_,
		Configuration::getMaxFramerate () );

	if ( framerateIsSet == 0 )
	{
		Log ( INFO ) << "Successfully started the framerate manager.";
	}
	else
	{
		Log ( WARN ) << "Failed to start the framerate manager.";
	}
}

/**
* Delays execution and calculates FPS.
* @note Only delays the execution if needed.
* @param fps_manager_ : The FPS manager to handle.
* @return The time in seconds since the last call.
*/
double FPSWrapper::delay ( FPSmanager& fps_manager_ )
{
	return ( double ) ( SDL_framerateDelay ( &fps_manager_ ) / 1000.0 );
}
