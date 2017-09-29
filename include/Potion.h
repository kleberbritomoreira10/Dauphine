#ifndef INCLUDE_POTION_H
#define INCLUDE_POTION_H

#include "DynamicEntity.h"
#include "Animation.h"
/**
* Class for the bomb potion.
*/
class Potion : public DynamicEntity
{
	public:
		/**
		* The constructor.
		*/
		Potion( const double x_, const double y_, const std::string& PATH, const int strength_, const int inertia_, 
			const int distance_, const bool isRight_ );

		/**
		* The destructor.
		*/
		virtual ~Potion();

		/**
		*
		*/
		virtual void update( const double DELTA_TIME );

		/**
		*
		*
		*/
		virtual void render( const double camera_position_x, const double camera_position_y );

		bool activated;

		Animation *getAnimation();

		bool canExplode;
		bool isExploding;

	private:
		/**
		*/
		virtual void handle_collision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_ );
		virtual void update_bounding_box();
		int strength;
		int distance;
		double flightTime;
		Animation *animation;

};

#endif // INCLUDE_BOMBPOTION_H
