/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Configuration.h
 * Game configuration class
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_H

#include <string>

/**
* Game configuration class.
* @todo Refactor the Configuration class.
* @todo Implement all the possible resolutions the game will "want".
*/
class Configuration
{

	public:
		/**
		* Initializes all the attributes.
		*/
		static void initialize ();

		/**
		* @return resolutionWidth
		*/
		static unsigned int get_resolution_width ();

		/**
		* @return resolutionHeight
		*/
		static unsigned int get_resolution_height ();

		/**
		* @return maxFramerate
		*/
		static uint32_t getMaxFramerate ();

		/**
		* @return window_title
		*/
		static std::string getWindowTitle ();

		/**
		* @return initialMultiplier
		* @see Window::rescale
		*/
		static unsigned int get_logical_render_size ();

		/**
		* @return cameraDistanceWidth
		*/
		static unsigned int getCameraDistanceWidth ();

		/**
		* @return cameraDistanceHeight
		*/
		static unsigned int getCameraDistanceHeight ();

		/**
		* @return The current screen width.
		*/
		static unsigned int getScreenWidth ();

		/**
		* @return The current screen height.
		*/
		static unsigned int getScreenHeight ();

	private:

		static const unsigned int resolutionWidth; // The game's width resolution. (16)
		static const unsigned int resolutionHeight; // The game's height resolution. (10)

		static uint32_t maxFramerate; // The game's max framerate.
		static std::string window_title; // The game window's title.

		static unsigned int logicalRenderSize; // The size of the logical rendering.

		static unsigned int screenWidth; // The screen width.
		static unsigned int screenHeight; // The screen height.

		static unsigned int cameraDistanceWidth; // The width distance of the camera.
		static unsigned int cameraDistanceHeight; // The height distance of the camera.

};

#endif //INCLUDE_CONFIGURATION_H
