#ifndef INCLUDE_LEVEL_H
#define INCLUDE_LEVEL_H

#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "StateGame.h"
#include "TileMap.h"
#include "QuadTree.h"
#include "PlayerHUD.h"
#include "Enemy.h"
#include "Document.h"
#include "Boss.h"

#include <vector>

/**
* Abstract class for levels.
* Contains a background, player and a camera.
*/
class Level : public StateGame
{

	public:
		/**
		* The destructor.
		* Deletes all the allocated attributes, even though the specific levels should.
		*/
		virtual ~Level();

		/**
		* @return The Level width.
		*/
		unsigned int getWidth();

		/**
		* @return The Level height.
		*/
		unsigned int getHeight();

	protected:
		/**
		* The constructor.
		* Initializes all the attributes.
		*/
		Level();

		/**
		* @param player_ : Sets the player for the level.
		*/
		virtual void set_player ( Player *const player_ );

		/**
		* @param camera_ : Sets the camera for the level.
		* @note You should only set the camera after setting the player. Will warn if you
		* 	didn't.
		*/
		virtual void set_camera ( Camera *const camera_ );

		virtual void setBoss ( Boss *const boss );

		virtual void clear_enemies();
		virtual void clear_documents();

		void changeCheckpoints ( int TOTAL_NUMBER_OF_CHECKPOINTS_, std::vector <double> checkpoints_X_,
		std::vector < double > checkpoints_Y_ );

		unsigned int width = 0; /**< Width that defines the horizontal limits. */
		unsigned int height = 0; /**< Height that defines the vertical limits. */

		Player *player; /**< The direct reference to player, even though its in the list. */
		Camera *camera; /**< The current camera for that level. */
		PlayerHUD *player_Hud; // The direct reference to player hud.

		Boss *boss; // The direct reference to the boss.

		TileMap *tile_map; // The direct reference to tile map.
		QuadTree *quadTree; // The direct reference to quadtree.

		Sprite *background; // Sprite of the background.
		Sprite *backgroud_top; // Sprite of backgroud top.
		std::vector < Sprite * > checkpoints; // Array of checkpoints sprites.
		std::vector < double > checkpoints_X; // Array of checkpoint in X axis.
		std::vector < double > checkpoints_Y; // Array of checkpoint in Y axis.
		std::vector < bool > checkpoints_visited; // Array of all checkpoints visited.
		int TOTAL_NUMBER_OF_CHECKPOINTS = 0; // Total number of checkpoints.

		std::vector <Enemy*> enemies; // Array of direct reference to the enemies.
		std::vector <Document*> documents; //Array of direct reference to documents.

		void delete_camera();

		void delete_player_hud();

		void delete_tile_map();

		void delete_quadTree();

};

#endif //INCLUDE_LEVEL_H
