#ifndef INCLUDE_BSTATEATTACK_H
#define INCLUDE_BSTATEATTACK_H

#include "StateBoss.h"

class BStateAttack : public StateBoss 
{

	public:
		/**
		* The constructor.
		* @param BOSS : Reference to the Boss.
		*/
		BStateAttack ( Boss *const BOSS );
		
		/**
		* The destructor.
		*/
		virtual ~BStateAttack () {}

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

};

#endif // INCLUDE_BSTATEATTACK_H
