/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @DynamicEntity.h
 * Subclass of Entity.
 * The objects of this class are entities, but have physics applied to them (such as velocity).
 * License: Copyright (C) 2014 Alke Games.
  */

#ifndef INCLUDE_DYNAMICENTITY_H
#define INCLUDE_DYNAMICENTITY_H

#include "Entity.h"
#include "Crosshair.h"
#include <array>

enum CollisionSide : uint8_t
{

	SOLID_TOP = 0,
	SOLID_BOTTOM,
	SOLID_RIGHT,
	SOLID_LEFT,
	SOLID_TOTAL

};

/**
* Subclass of Entity.
* The objects of this class are entities, but have physics applied to them (such as velocity).
*/
class DynamicEntity : public Entity
{

	public:
		/**
		* The constructor.
		* Initializes all attributes.
		* @param x_ : position in x axis.
		* @param y_ : position in y axis.
		* @param sprite_ : which sprite to use.
		*/
		DynamicEntity ( const double x_, const double y_, const std::string &PATH );

		/**
		* The destructor.
		*/
		virtual ~DynamicEntity ();

		virtual void jump ();
		virtual void applyGravity ();
		virtual void move ( const bool movingLeft_, const bool movingRight_ );
		virtual void moveVertical ( const bool movingUp_, const bool movingDown_ );
		virtual void slowVx ();
		virtual void slowVy ();
		virtual void roll ();
		virtual void aim ( Crosshair *const crosshair, double direction );

		/**
		* @param width_,height_ : Tells the player what the width and height of the level is.
		* @see Level::update()
		*/
		virtual void setLevelWH ( const unsigned int width_, const unsigned int height_ );

		double velocity_x_axis; // The dynamic entity's speed on the x axis.
		double velocity_y_axis; // The dynamic entity's speed on the y axis.
		double speed; // Speed that moves dynamic entity on input.
		double maxSpeed; // Dynamic entity max speed.
		bool isGrounded; // Check for if the dynamic entity is on the ground.
		double nextX; // Used to update next position on x-axis.
		double nextY; // Used to update next position on y-axis.
		bool reached_level_end; // Boolean value to evaluate the level end.

	    bool activated;
	    int strength;
	    int distance;
	    double flightTime;

	    bool isClimbing; // Boolean value evaluate the character's climbing condition

	protected:
		/**
		* Updates the position of the dynamic entity
		* Update is based on what input was recieved, and the players velocity.
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
		* 	of processing speed.
		*/
		virtual void updatePosition ( const double DELTA_TIME );

		virtual void scoutPosition ( const double DELTA_TIME );

		/**
		* @return A bool array with the sides the DynamicEntity collided.
		*/
		virtual std::array < bool, CollisionSide::SOLID_TOTAL > detectCollision ();

		/**
		* Handles the collision.
		*/
		virtual void handleCollision (std::array < bool, CollisionSide::SOLID_TOTAL > detections_ ) = 0;

		virtual SDL_RendererFlip getFlip ();

		virtual void updateBoundingBox () = 0;

		unsigned int levelW; // The width of the level.
		unsigned int levelH; // The height of the level.

};

#endif // INCLUDE_DYNAMICENTITY_H
