/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Player.cpp
* The player entity class.
* Contains all the relevant implementation relative to the player.
* License: Copyright (C) 2014 Alke Games.*/

#include "Player.h"
#include "Game.h"
#include "Logger.h"
#include "Configuration.h"
#include "LuaScript.h"
#include <cmath>
#include <cfloat>

#include "PStateIdle.h"
#include "PStateAerial.h"
#include "PStateMoving.h"
#include "PStateRolling.h"
#include "PStateCrouching.h"
#include "PStateAiming.h"
#include "PStateMovingCrouch.h"
#include "PStateAttack.h"
#include "PStateAttackMoving.h"
#include "PStateAttackJumping.h"
#include "PStateHit.h"
#include "PStateClimbing.h"
#include "PStateDead.h"

#include "Window.h"

#define ADD_STATE_EMPLACE( stateEnum, stateClass ) this  -> states_map.emplace( stateEnum, new stateClass( this ) )
#define ADD_STATE_INSERT( stateEnum, stateClass ) this -> states_map.insert( std::make_pair<player_states, StatePlayer*>( stateEnum, new stateClass( this ) ) );

/**
* The constructor.
* Creates the player by setting the position and sprite.
* @param x_ : position in x axis.
* @param y_ : position in y axis.
* @param sprite_ : which sprite to use.
*/
Player::Player( const double x_, const double y_, const std::string &PATH ) :
    DynamicEntity( x_, y_, PATH ),
    potions_left( 3 ),
    maxPotions( 3 ),
    crosshair( new Crosshair(0.0, 0.0, "res/images/alvo.png" )),
    life( 3 ),
    attack_strength( 50 ),
    can_attack( true ),
    currentItem(PItems::POTION),
    closestEnemyIsRight( true ),
    is_vulnerable( true ),
    invulnerableTime( 0 ),
    canMove( true ),
    animation( nullptr ),
    current_state( nullptr )
{
    initialize_states();

    // Loading lua modules.
    LuaScript luaPlayer( "lua/Player.lua" );
    this ->  width = luaPlayer.unlua_get<int>( "player.dimensions.width" );
    this -> height = luaPlayer.unlua_get<int>( "player.dimensions.height" );

    // Loading animation.
    this -> animation = new Animation( 0, 3, this -> width, this -> height, 11, false );

    // Verifying if the sprite is available.
    if ( this -> sprite != nullptr )
    {
        this -> current_state = this -> states_map.at( IDLE );
        this -> current_state -> enter();
    } else
    {
        Log( WARN ) << "No sprite set for the player! Null sprite.";
    }

}

/**
* The destructor.
* Exits the current state and destroys all states.
*/
Player::~Player()
{

    // Exiting the current state.
    if ( this -> current_state != nullptr )
    {
        this -> current_state -> exit();
        this -> current_state = nullptr;
    }

    // Deleting animation.
    if ( this -> animation != nullptr )
    {
        delete this -> animation;
        this -> animation = nullptr;
    }

    destroy_states();
}

/**
* Updates the player.
* @see Player::updateInput, Player::update_position
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
*   of processing speed.
*/
void Player::update( const double DELTA_TIME )
{

    // Getting inputs to update the player.
    std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

    // Vrifying if the player can move.
    if ( this -> canMove )
    {
        this -> current_state -> handleInput( keyStates );
    }

    // Updating the actions.
    Game::instance().clearKeyFromInput( GameKeys::ACTION );
    scout_position( DELTA_TIME );
    update_bounding_box();

    // Loading collision.
    const std::array<bool, CollisionSide::SOLID_TOTAL> detections = detect_collision();
    handle_collision( detections );
    update_position( DELTA_TIME );

    // Updating animation.
    this -> animation -> update( this -> animation_clip, DELTA_TIME );

    // Updating potions if activated.
    for ( auto potion : this -> potions )
    {
        if ( !potion -> activated )
        {
            // Delete potion.
        }
        potion -> update(DELTA_TIME);
    }

    // Verifying if the player is vulnerable, if not, the player can attack.
    if ( !this -> is_vulnerable )
    {
        this -> invulnerableTime += DELTA_TIME;
        if ( this -> invulnerableTime >= 1 )
        {
            this -> invulnerableTime = 0;
            this -> is_vulnerable = true;
            this -> can_attack = true;
        }
    }

    // Verifying if player is climbing.
    if ( this -> isClimbing && !is_current_state(player_states::CLIMBING) )
    {
        change_state( player_states::CLIMBING );
    }

}

/**
* Handle player's collision.
  @param detections_: Array of detected collisions.
*/
void Player::handle_collision( std::array<bool, CollisionSide::SOLID_TOTAL> detections_ )
{
    // Verifying if the collision is COLLIDED_TOP.
    if ( detections_.at(CollisionSide::SOLID_TOP) )
    {
        Log(DEBUG) << "COLLIDED_TOP";
        this -> velocity_y_axis = 0.0;
    }

    // Verifying if the collision is SOLID_BOTTOM.
    if ( detections_.at(CollisionSide::SOLID_BOTTOM) )
    {
        if ( is_current_state( player_states::AERIAL ) || is_current_state( player_states::ATTACKJUMPING )
            || is_current_state( player_states::HITED )  || is_current_state( player_states::CLIMBING ) ||
            is_current_state( player_states::DEAD ) )
            {
            const double magic = 32.0;
            const double aerialToIdleCorrection = 8.0;

            this -> nextY -= fmod( this -> nextY, 64.0 ) - magic + aerialToIdleCorrection;
            this -> velocity_y_axis = 0.0;
            if ( !is_current_state( player_states::DEAD ) )
            {
                change_state( player_states::IDLE );
            }
        }
    } else
    {
        // Changing the player's state.
        if ( !is_current_state( player_states::AERIAL ) && !is_current_state( player_states::ATTACKJUMPING )
            && !is_current_state( player_states::CLIMBING ) && !is_current_state( player_states::DEAD ))
        {
            change_state( player_states::AERIAL );
        }
    }

    //Verifying if the collision side is left.
    if ( detections_.at( CollisionSide::SOLID_LEFT ) )
    {
        this -> next_position_x = this -> x;
        this -> velocity_x_axis = 0.0;
    }

    // Verifying if the collision side is right.
    if ( detections_.at( CollisionSide::SOLID_RIGHT ) )
    {
        this -> next_position_x = this -> x;
        this -> velocity_x_axis = -0.001;
    }

}

/**
* Renders the player.
* Uses the player's sprite render method.
* @see Sprite::render
* @param camera_position_x : The x position of the camera.
* @param camera_position_y : The y position of the camera.
*/
void Player::render( const double camera_position_x, const double camera_position_y )
{
    const double dx = this -> x - camera_position_x;
    const double dy = this -> y - camera_position_y;

    /*Actual.
    SDL_Rect actualRect = {(int)dx, (int)dy, (int)this -> width, (int)this -> height};
    SDL_SetRenderDrawColor( Window::getRenderer(), 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(Window::getRenderer(), &actualRect);

    Bounding box.
    SDL_Rect boundingBox2 = {(int)(this -> boundingBox.x - camera_position_x), (int)(this -> boundingBox.y - camera_position_y), (int)this -> boundingBox.w, (int)this -> boundingBox.h};
    SDL_SetRenderDrawColor( Window::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(Window::getRenderer(), &boundingBox2); */

    // Rendering SDL_FLIP.
    if ( this -> sprite != nullptr )
    {
        SDL_RendererFlip flip = get_flip();

        if ( flip == SDL_FLIP_HORIZONTAL )
        {
            this -> sprite -> render( dx - 100, dy, &this -> animation_clip, false, 0.0, nullptr, flip );
        } else
        {
            this -> sprite -> render( dx, dy, &this -> animation_clip, false, 0.0, nullptr, flip );
        }
    }

    // Rendering crosshair.
    if ( this -> crosshair != nullptr )
    {
        this -> crosshair -> render( camera_position_x, camera_position_y );
    }

    // Rendering potions.
    for ( auto potion : this -> potions )
    {
        potion -> render( camera_position_x, camera_position_y );
    }

}

/**
* Use player's potion.
  @param strength_: The strength of the potion.
  @param distance_: The distance of the potion.
*/
void Player::usePotion( const int strength_, const int distance_ )
{

    // Using potions if the potions' quantity is > 0.
    if ( this -> potions_left > 0 )
    {
        this -> potions_left--;
        const double potionX = (( this -> is_right ) ? this -> boundingBox.x + this -> boundingBox.w : this -> boundingBox.x );
        Potion *potion = new Potion( potionX , this -> y, "res/images/explosion_with_potion.png",
        strength_, this -> velocity_x_axis, distance_, this -> is_right );
        this -> potions.push_back( potion );
    }
}

/**
* Add player's potion.
  @param quantity_: Quantity of potions.
*/
void Player::addPotions( const unsigned int quantity_ )
{
    // Adding potions if the total of potions is < maximum number of potions.
    if ( this -> potions_left + quantity_ > this -> maxPotions )
    {
        this -> potions_left = this -> maxPotions;
    } else
    {
        this -> potions_left += quantity_;
    }
}

/**
* Loads all the states.
* Every new state implemented should be INITIALIZED here.
*/
void Player::initialize_states()
{
    // Loading all the states.
    ADD_STATE_INSERT( IDLE,         PStateIdle );
    ADD_STATE_INSERT( MOVING,       PStateMoving );
    ADD_STATE_INSERT( AERIAL,       PStateAerial );
    ADD_STATE_INSERT( ROLLING,      PStateRolling );
    ADD_STATE_INSERT( CROUCHING,    PStateCrouching );
    ADD_STATE_INSERT( AIMING,       PStateAiming );
    ADD_STATE_INSERT( MOVINGCROUCH, PStateMovingCrouch );
    ADD_STATE_INSERT( ATTACK,       PStateAttack );
    ADD_STATE_INSERT( ATTACKMOVING, PStateAttackMoving );
    ADD_STATE_INSERT( ATTACKJUMPING,PStateAttackJumping );
    ADD_STATE_INSERT( HITED,        PStateHit );
    ADD_STATE_INSERT( CLIMBING,     PStateClimbing );
    ADD_STATE_INSERT( DEAD,         PStateDead );
}

/**
* Deletes all the loaded states.
* Every new state implemented should be deleted here.
*/
void Player::destroy_states()
{
    // Delete all the states in Player here.
    std::map<player_states, StatePlayer*>::const_iterator it;
    for ( it = this -> states_map.begin(); it != this -> states_map.end(); it++ )
    {
        delete it -> second;
    }
}

/**
* Deletes all the loaded states.
* Every new state implemented should be deleted here.
  @param state_: The state to be changed.
*/
void Player::change_state( const player_states state_ )
{
    this -> current_state -> exit();
    this -> current_state = this -> states_map.at( state_ );
    this -> current_state -> enter();
}

/**
* Get the player's animation.
* @return The players current animation setting.
*/
Animation *Player::getAnimation()
{
    return ( this -> animation );
}

/**
  @param state_: The state of the player.
* @return Whether the player is currently in player_states::state_ or not.
*/
bool Player::is_current_state(const player_states state_)
{
    return ( this -> current_state == this -> states_map.at(state_) );
}

/**
* Update the bounding box.
*/
void Player::update_bounding_box()
{
    this -> boundingBox.x = (int) this -> next_position_x + this -> current_state -> box.x;
    this -> boundingBox.y = (int) this -> nextY + this -> current_state -> box.y;
    this -> boundingBox.w = this -> current_state -> box.w;
    this -> boundingBox.h = this -> current_state -> box.h;
}

/**
* Verify is the player is dead.
* @return: Return true or false if the player is dead.
*/
bool Player::is_dead()
{
    return ( this -> life <= 0 );
}
