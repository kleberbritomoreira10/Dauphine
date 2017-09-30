#ifndef INCLUDE_ENTITY_H
#define INCLUDE_ENTITY_H

#include "Sprite.h"
#include "SDLWrapper.h"
#include <vector>
#include "CollisionRect.h"

/**
* Base class for all game objects (entities).
* The entity contains the (x,y) position, width and height, and a sprite.
*/
class Entity 
{

	public:
		/**
		* The destructor.
		*/
		virtual ~Entity ();

		/**
		* Updates the entity.
		* Pure virtual function. Purpose is to update whatever is necessary, relative to the
		* 	entity.
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double DELTA_TIME ) = 0;

		/**
		* Renders the entity.
		* Pure virtual function. Purpose is to copy the entity's texture onto the renderer.
		* @note Usually just calls the render method from the Sprite class.
		* @param camera_position_x : The x position of the camera.
		* @param camera_position_y : The y position of the camera.
		*/
		virtual void render ( const double camera_position_x, const double camera_position_y ) = 0;

		/**
		* @return The Entity width.
		*/
		unsigned int getWidth ();

		/**
		* @return The Entity height.
		*/
		unsigned int getHeight ();

		/**
		* @return The Entity::animation_clip.
		*/
		SDL_Rect& get_animation_clip ();
 
		SDL_Rect& get_bounding_box ();

		virtual void set_collision_rects ( const std::vector < CollisionRect > &collisionRects_ );

		double x; /**< The position in the x axis. */
		double y; /**< The position in the y axis. */
		bool is_right; /**< The direction in the x axis. */ 

	protected:
		/**
		* The constructor.
		* Initializes all the attributes.
		* @param x_ : position in x axis.
		* @param y_ : position in y axis.
		* @param sprite_ : which sprite to use.
		*/
		Entity ( const double x_, const double y_, const std::string &PATH );

		/**
		* The constructor.
		* Initializes all the attributes.
		* @param x_ : position in x axis.
		* @param y_ : position in y axis.
		*/
		Entity ( const double x_, const double y_ );

		Sprite *sprite; /**< The Sprite attributed to the entity. */
		unsigned int width; /**< The entitys width, from its sprite. */
		unsigned int height; /**< The entitys height, from its sprite. */
		SDL_Rect animation_clip; /**< The current clip on the spritesheet, to determine animation. */
		SDL_Rect bounding_box;

		std::vector < CollisionRect > collisionRects; /**< The SDL_Rects containing collidable tiles 
			positions. */

};

#endif //INCLUDE_ENTITY_H
