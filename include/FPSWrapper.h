/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FPSWrapper.cpp
 * Objects in this class are responsible for FPS manager and sets framerate.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_FPSWRAPPER_H
#define INCLUDE_FPSWRAPPER_H

#include "SDLWrapper.h"

/**
* Wraps FPS handling in a class.
* Uses SDL2_GFX, by ferzkopp.
*/
class FPSWrapper
{

	public:
		/**
		* Initializes FPS managing.
		* Initializes the FPS manager and sets it to the desired framerate
		* 	(Configuration::maxFramerate).
		* @param fpsManager_ : The FPS manager to handle.
		* @note Will warn if failed to set FPS.
		*/
		static void initialize ( FPSmanager& fpsManager_ );

		/**
		* Delays execution and calculates FPS.
		* @note Only delays the execution if needed.
		* @param fpsManager_ : The FPS manager to handle.
		* @return The time in seconds since the last call.
		*/
		static double delay ( FPSmanager& fpsManager_ );

};

#endif //INCLUDE_FPSWRAPPER_H
