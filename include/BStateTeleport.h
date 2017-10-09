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
		* @param BOSS : Reference to the Boss.
		*/
		BStateTeleport ( Boss *const BOSS );

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
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other
		*/
		virtual void update ( const double DELTA_TIME );

};

#endif // INCLUDE_BSTATETELEPORT_H
