#include "Crosshair.h"

Crosshair::Crosshair(const double x_, const double y_, const std::string& path_) :
	Entity(x_, y_, path_),
	activated(false)
{

}

Crosshair::~Crosshair(){

}

void Crosshair::update(const double DELTA_TIME){
	((void)DELTA_TIME); // Unused.
}
		
void Crosshair::render(const double cameraX_, const double cameraY_){
	if(this->activated && this->sprite != nullptr){
        const double dx = this->x - cameraX_;
        const double dy = this->y - cameraY_;
        this->sprite->render(dx, dy, nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
    }
}
unsigned int Crosshair::getWidth(){
	return this->width;
}

unsigned int Crosshair::getHeight(){
	return this->height;
}
