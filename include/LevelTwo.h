#ifndef INCLUDE_LEVELTWO_H
#define INCLUDE_LEVELTWO_H

#include "Level.h"
#include "Sprite.h"
#include "CollisionRect.h"

#define NUMBER_ITEMS 4

/**
* The first level of the game.
* Derived from Level class.
*/
class LevelTwo : public Level 
{
	
	public:
		/**
		* The constructor.
		* @see Level::Level()
		*/
		LevelTwo ();

		/**
		* The destructor.
		*/
		virtual ~LevelTwo ();

		/**
		* Loads the level.
		* From the Level1.lua script, loads all the necessary objects.
		*/
		virtual void load ();

		virtual Player* create_player();

		virtual void load_enemy ();

		virtual void save();

		virtual void document_check();

		virtual void update_collision();

		virtual void update_potion();

		virtual void update_number_potion();

		virtual std::vector<CollisionRect> update_entity (std::vector<CollisionRect> return_objects, const double DELTA_TIME);

		/**
		* Updates the objects within the Level.
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double DELTA_TIME );

		/**
		* Unloads everything that was loaded.
		* @see LevelTwo::load()
		*/
		virtual void unload ();

		/**
		* Renders the level.
		* Always renders on 0,0 position.
		* @see Sprite::render()
		*/
		virtual void render ();

		int items [ 2 ] [ NUMBER_ITEMS ];
		Sprite *image;
		bool caught_items [ NUMBER_ITEMS ];

};

#endif //INCLUDE_LEVELTWO_H
