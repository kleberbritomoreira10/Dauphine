#ifndef INCLUDE_ESTATECURIOUS_H
#define INCLUDE_ESTATECURIOUS_H

#include "StateEnemy.h"

class EStateCurious : public StateEnemy
{

	public:
		/**
		* The constructor.
		* @param enemy_ : Reference to the Enemy.
		*/
		EStateCurious ( Enemy *const enemy_ );

		/**
		* The destructor.
		*/
		virtual ~EStateCurious () {}

		/**
		* @see StateEnemy::enter
		*/
		virtual void enter ();

		/**
		* @see StateEnemy::exit
		*/
		virtual void exit ();

		/**
		* @see StateEnemy::update
		*/
		virtual void update ( const double DELTA_TIME );

	private:
		double time_elapsed; // The elapsed time of the enemy watching the player to return to patrol.

};

#endif // INCLUDE_ESTATECURIOUS_H
