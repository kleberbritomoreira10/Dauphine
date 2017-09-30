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
		* @see Player::updateInput, Player::update_position
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
		* 	of processing speed.
		*/
		virtual void update( const double DELTA_TIME );

		/**
		* Renders the player.
		* Uses the player's sprite render method.
		* @see Sprite::render
		* @param camera_position_x : The x position of the camera.
		* @param camera_position_y : The y position of the camera.
		*/
		virtual void render( const double camera_position_x, const double camera_position_y );

		/**
		* Loads all the states.
		* Every new state implemented should be INITIALIZED here.
		*/
		void initialize_states();

		/**
		* Deletes all the loaded states.
		* Every new state implemented should be deleted here.
		*/
		void destroy_states();

		/**
		* Sets the current game state.
		* @see StatePlayer::load
		* @see StatePlayer::unload
		* @param state_ : The state you want to be changed into. All states are inside Player.
		*/
		void change_state( const player_states state_ );

		/**
		* @return Whether the player is currently in player_states::state_ or not.
		*/
		bool is_current_state( const player_states state_ );

		/**
		* @return The players current animation setting.
		*/
		Animation *get_animation();

		void usePotion( const int strength_, const int distance_ );
		void addPotions( const unsigned int quantity_ );

		unsigned int potions_left;
		unsigned int maxPotions;
		std::vector<Potion*> potions;

		Crosshair *crosshair;

		unsigned int life;
		unsigned int attack_strength;
		//unsigned int maxLife;

		bool can_attack;
		unsigned int currentItem;

		bool is_dead();
		bool closestEnemyIsRight;

		bool is_vulnerable;
		double invulnerableTime;
		bool canMove;

	private:
		virtual void update_bounding_box();
		virtual void handle_collision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_ );

		Animation *animation; /**< Current player animation. */
		StatePlayer *current_state; /**< The current state, which the player is in. */
		std::map<player_states, StatePlayer*> states_map; /**< Map containing all possible states. */

};

#endif //INCLUDE_PLAYER_H
