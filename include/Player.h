#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

#include "DynamicEntity.h"
#include "Sprite.h"
#include "InputKeys.h"
#include "StatePlayer.h"
#include "Animation.h"
#include "Crosshair.h"
#include "Potion.h"
#include <map>
#include <vector>

class StatePlayer;

class Potion;
/**
* The player entity class.
* Contains all the relevant implementation relative to the player.
*/
class Player : public DynamicEntity
{

	public:
		/**
		* All possible player states.
		*/
		enum player_states : uint8_t
		{
			IDLE = 0,
			MOVING,
			AERIAL,
			ROLLING,
			CROUCHING,
			AIMING,
			MOVINGCROUCH,
			ATTACK,
			ATTACKMOVING,
			ATTACKJUMPING,
			HITED,
			CLIMBING,
			DEAD
		};

		enum PItems : uint8_t
		{
			POTION = 0
		};

		/**
		* The constructor.
		* Creates the player by setting the position and sprite.
		* @param x_ : position in x axis.
		* @param y_ : position in y axis.
		* @param sprite_ : which sprite to use.
		*/
		Player( const double x_, const double y_, const std::string& PATH );

		/**
		* The destructor.
		* Exits the current state and destroys all states.
		*/
		virtual ~Player();

		/**
		* Updates the player.
		* @see Player::updateInput, Player::updatePosition
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
		* 	of processing speed.
		*/
		virtual void update( const double DELTA_TIME );

		/**
		* Renders the player.
		* Uses the player's sprite render method.
		* @see Sprite::render
		* @param cameraX_ : The x position of the camera.
		* @param cameraY_ : The y position of the camera.
		*/
		virtual void render( const double cameraX_, const double cameraY_ );

		/**
		* Loads all the states.
		* Every new state implemented should be INITIALIZED here.
		*/
		void initializeStates();

		/**
		* Deletes all the loaded states.
		* Every new state implemented should be deleted here.
		*/
		void destroyStates();

		/**
		* Sets the current game state.
		* @see StatePlayer::load
		* @see StatePlayer::unload
		* @param state_ : The state you want to be changed into. All states are inside Player.
		*/
		void changeState( const player_states state_ );

		/**
		* @return Whether the player is currently in player_states::state_ or not.
		*/
		bool is_current_state( const player_states state_ );

		/**
		* @return The players current animation setting.
		*/
		Animation *getAnimation();

		void usePotion( const int strength_, const int distance_ );
		void addPotions( const unsigned int quantity_ );

		unsigned int potionsLeft;
		unsigned int maxPotions;
		std::vector<Potion*> potions;

		Crosshair *crosshair;

		unsigned int life;
		unsigned int attackStrength;
		//unsigned int maxLife;

		bool can_attack;
		unsigned int currentItem;

		bool isDead();
		bool closestEnemyIsRight;

		bool is_vulnerable;
		double invulnerableTime;
		bool canMove;

	private:
		virtual void updateBoundingBox();
		virtual void handleCollision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_ );

		Animation *animation; /**< Current player animation. */
		StatePlayer *current_state; /**< The current state, which the player is in. */
		std::map<player_states, StatePlayer*> statesMap; /**< Map containing all possible states. */

};

#endif //INCLUDE_PLAYER_H
