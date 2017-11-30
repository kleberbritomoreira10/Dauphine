#include "Collision.h"
#include <cmath>
#include "Logger.h"

bool Collision::rects_collided(const SDL_Rect& a_, const SDL_Rect& b_){
    // Calculate the sides of rect A.
    const int leftA = a_.x;
    const int rightA = a_.x + a_.w;
    const int topA = a_.y;
    const int bottomA = a_.y + a_.h;

    // Calculate the sides of rect B.
    const int leftB = b_.x;
    const int rightB = b_.x + b_.w;
    const int topB = b_.y;
    const int bottomB = b_.y + b_.h;

    // If any of the sides from A are outside of B.
    if(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
        return false;
    }
    // If none of the sides from A are outside B.
    else{
        return true;
    }

}

Collision::RectangleSide Collision::rectsCollidedSide(const SDL_Rect& a_, const SDL_Rect& b_){
    const double w = 0.5 * (a_.w + b_.w);
    const double h = 0.5 * (a_.h + b_.h);

    const double centerAx = a_.x + (a_.w / 2);
    const double centerAy = a_.y + (a_.h / 2);
    const double centerBx = b_.x + (b_.w / 2);
    const double centerBy = b_.y + (b_.h / 2);

    const double dx = centerAx - centerBx;
    const double dy = centerAy - centerBy;

    if (abs(dx) <= w && abs(dy) <= h){
        /* collision! */
        const double wy = w * dy;
        const double hx = h * dx;

        if (wy > hx){
            if (wy > -hx){
                return RectangleSide::TOP;
                Log( INFO ) << "Returning the collision at the top of the rectangle";
            }
            else{
                return RectangleSide::LEFT;
                Log( INFO ) << "Returning the collision at the left of the rectangle";
            }
        }
        else{
            if (wy > -hx){
                return RectangleSide::RIGHT;
                Log( INFO ) << "Returning the collision at the right of the rectangle";
            }
            else{
                return RectangleSide::BOTTOM;
                Log( INFO ) << "Returning the collision at the bottom of the rectangle";
            }
        }
    }
    else{
        return RectangleSide::NONE;
        Log( DEBUG ) << "No collision returning";
    }
}
