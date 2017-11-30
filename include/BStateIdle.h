#ifndef INCLUDE_BSTATEIDLE_H
#define INCLUDE_BSTATEIDLE_H

#include "StateBoss.h"

class BStateIdle : public StateBoss
{

	public:
		/**
		* The constructor.
		* @param BOSS : Reference to the Boss.
		*/
		BStateIdle( Boss *const BOSS );

		/**
		* The destructor.
		*/
		virtual ~BStateIdle () {}

		/**
		* @see StateBoss::enter
		*/
		virtual void enter ();

		/**
		* @see StateBoss::exit
		*/
		virtual void exit ();

		/**
		* @see StateBoss::update
		*/
		virtual void update ( const double DELTA_TIME );

		void handle_boss_position();

		void handle_boss_movements();

};

#endif // INCLUDE_BSTATEIDLE_H
