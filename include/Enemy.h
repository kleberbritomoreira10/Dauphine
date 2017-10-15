/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Enemy.h
 * File responsible for implementing the characteristics of common enemies (except the boss).
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_ENEMY_H
#define INCLUDE_ENEMY_H

#include "DynamicEntity.h"
#include "StateEnemy.h"
#include "Animation.h"
#include <map>

class StateEnemy;

/*
 * Enemies characteristics.
 */
class Enemy : public DynamicEntity
{

	public:
		enum EStates : uint8_t
		{

			IDLE = 0,
			AERIAL,
			PATROLLING,
			CURIOUS,
			ALERT,
			ATTACK,
			DEAD

		};

		/**
		* The constructor.
		*/
		Enemy ( const double x_, const double y_, const std::string& PATH, const bool patrol_,
			const double patrolLength_ );

		/**
		* The destructor.
		*/
		virtual ~Enemy ();

		/**
		* Updates the player.
		* @see Player::updateInput, Player::updatePosition
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
		* 	of processing speed.
		*/
		virtual void update ( const double DELTA_TIME );

		/**
		* Renders the player.
		* Uses the player's sprite render method.
		* @see Sprite::render
		* @param cameraX_ : The x position of the camera.
		* @param cameraY_ : The y position of the camera.
		*/
		virtual void render ( const double cameraX_, const double cameraY_ );

		void initializeStates ();
		void destroyStates ();
		void changeState ( const EStates state_ );
		Animation* getAnimation ();
		bool isDead ();
		void set_dead ( bool isDead_ );

		static double px; // Position of the enemy in the origin of the x axis.
		static double py; // Position of the enemy in the origin of the y axis.
		static unsigned int points_life; // Reference to the enemy's life number.
		static bool position_vulnerable; // Boolean condition to check if enemy is position vulnerable.
		static double alert_range; // The value of the range for the enemy to be in alert mode when the player approaches.
		static double curious_range; // The value of the range for the enemy to be in curious mode when the player approaches.

		double original_X; // Original position on the x axis of the enemy;
		bool patrol;
		double patrol_length; // The space traveled by the patrolman.
		unsigned int life;

	private:

		virtual void updateBoundingBox ();
		virtual void handleCollision ( std::array < bool, CollisionSide::SOLID_TOTAL > detections_ );

		void forceMaxSpeed ();

		StateEnemy *current_state;
		Animation *animation;

		std::map < EStates, StateEnemy *> states_map;
		bool dead;

};

#endif // INCLUDE_ENEMY_H
