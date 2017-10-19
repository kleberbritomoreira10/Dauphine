/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Boss.h
 * File responsible for implementing the characteristics of the boss.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_BOSS_H
#define INCLUDE_BOSS_H

#include "DynamicEntity.h"
#include "StateBoss.h"
#include "Animation.h"
#include "Potion.h"
#include <map>
#include <vector>
#include "Player.h"

class StateBoss;

/*
 * Boss characteristics.
 */
class Boss : public DynamicEntity
{
  public:
    enum BStates : uint8_t
    {
      IDLE = 0,
      ATTACK,
      SHIELD,
      TELEPORT,
      ICEPRISION,
      MAGICPROJECTILE,
      DEAD
    };

    /**
     * The constructor.
     */
    Boss ( const double x_, const double y_, const std::string &PATH, Player *const player_ );

    /**
     * The destructor.
     */
    virtual ~Boss ();

    /**
     * Updates the player.
     * @see Player::updateInput, Player::updatePosition
     * @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
     *   of processing speed.
     */
    virtual void update ( const double DELTA_TIME );

    void initializeStates ();
    void destroyStates ();
    void changeState ( const BStates state_ );

    /**
     * Renders the player.
     * Uses the player's sprite render method.
     * @see Sprite::render
     * @param cameraX_ : The x position of the camera.
     * @param cameraY_ : The y position of the camera.
     */
    virtual void render ( const double cameraX_, const double cameraY_ );

    void usePotion ( const int strength_, const int distance_ );

    Animation *getAnimation ();
    bool isDead ();
    void set_dead ( bool isDead_ );

    enum BossSkills : uint8_t
    {
      BS_MAGIC_SHIELD = 0,
      BS_TELEPORT,
      BS_MAGIC_PROJECTILE,
      BS_INVOKE_WIND,
      BS_ICE_PRISION,
      BS_FINAL_SPLENDOR
    };

    void randomSkill ( const unsigned int index_ );
    bool teleport ();
    bool magicProjectile ();
    bool invokeWind ();
    bool icePrision ();
    bool finalSplendor ();

    unsigned int potions_left;

    bool saw_player; // Boleean condition to know if the boss saw the player.
    std::vector < Potion *> potions;

    unsigned int life; // Reference to Boss's life.
    bool has_shield; // Condition to the shield of the boss
    bool can_walk; // Condition for the boss to be able to walk.
    Player *player; // Reference to the Player.
    Animation *power_animation; // Reference to animation of Boss's power.
    double power_X_axis; // Power in the x-axis.
    double power_Y_axis; // Power in the y-axis.
    bool power_is_activated; // Boleean condition to activated the power.
    Sprite *power; // Referece to visual represetation of Boss power.
    SDL_Rect power_clip; // A structure that contains the definition of a rectangle, with the origin at the upper left.
    SDL_RendererFlip power_flip; // An enumeration of flags that can be used in the flip parameter.
    Animation *shield_animation;// Reference to animation shield.
    Sprite *shield; // Referece to visual represetation of Boss shield.
    SDL_Rect shield_clip; // A structure that contains the definition of a rectangle, with the origin at the upper left.

  private:
    virtual void updateBoundingBox ();
    virtual void handleCollision ( std::array < bool, CollisionSide::SOLID_TOTAL > detections_ );

    StateBoss *current_state; // Get the current state of the boss.
    Animation *animation; // Copy to the animation of the boss.
    std::map < BStates, StateBoss * > states_map;
    bool dead; // Boleean condition to death os the boss.
};

#endif // INCLUDE_BOSS_H
