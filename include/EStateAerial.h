#ifndef INCLUDE_ESTATEAERIAL_H
#define INCLUDE_ESTATEAERIAL_H

#include "StateEnemy.h"

class EStateAerial : public StateEnemy 
{

	public:
		/**
		* The constructor.
		* @param enemy_ : Reference to the Enemy.
		*/
		EStateAerial ( Enemy *const enemy_ );
		
		/**
		* The destructor.
		*/
		virtual ~EStateAerial (){}

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

};

#endif // INCLUDE_ESTATEAERIAL_H
