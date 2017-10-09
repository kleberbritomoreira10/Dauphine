#ifndef INCLUDE_FADEHANDLER_H
#define INCLUDE_FADEHANDLER_H

#include "Sprite.h"

class FadeHandler
{

	public:
		FadeHandler( Sprite *const sprite_ );
		virtual ~FadeHandler ();

		void fadeIn ( const double percentage_, const double time_ );
		void fade_out ( const double percentage_, const double time_ );
		void update ( const double DELTA_TIME );
		double getCurrentPercentage ();

	private:

		bool should_fade_in; // Boleean condition to fade in the screen.
		bool should_fade_out; // Boleean condition to fade out in the screen.
		double stop_percentage; // Percentage value for fade stop.
		Sprite* sprite; // Reference to sisual representation of game objects.
		double time; // Time to fade out or fade in.
		double current_percentage; // Percentage value of current fade on screen.
		double rate; // Rate to fade out or fade in.

};

#endif // INCLUDE_FADEHANDLER_H
