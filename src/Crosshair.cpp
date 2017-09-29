#include "Crosshair.h"

Crosshair::Crosshair(const double x_, const double y_, const std::string& PATH) :
	Entity(x_, y_, PATH),
	activated(false)
{

}

Crosshair::~Crosshair(){

}

void Crosshair::update(const double DELTA_TIME){
	((void)DELTA_TIME); // Unused.
}
		
void Crosshair::render(const double camera_position_x, const double camera_position_y){
	if(this->activated && this->sprite != nullptr){
        const double dx = this->x - camera_position_x;
        const double dy = this->y - camera_position_y;
        this->sprite->render(dx, dy, nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}
unsigned int Crosshair::getWidth(){
	return this->width;
}

unsigned int Crosshair::getHeight(){
	return this->height;
}
