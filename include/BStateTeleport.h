/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @BStateTeleport.h
 * Objects in this class control the behavior of teleportation.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_BSTATETELEPORT_H
#define INCLUDE_BSTATETELEPORT_H

#include "StateBoss.h"

/*

*/
class BStateTeleport : public StateBoss
{

	public:
		/**
		* The constructor.
		* @param boss_ : Reference to the Boss.
		*/
		BStateTeleport ( Boss *const boss_ );

		/**
		* The destructor.
		*/
		virtual ~BStateTeleport () {}

		/**
		* @see StateBoss::enter
		*/
		virtual void enter ();

		/**
		* @see StateBoss::exit
		*/
		virtual void exit ();

		/**
		* @see StateBoss::
		* @param dt_ : Delta time. Time elapsed between one frame and the other
		*/
		virtual void update ( const double dt_ );

};

#endif // INCLUDE_BSTATETELEPORT_H
