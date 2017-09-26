/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @CollisionRect.cpp
 * File responsible for checking if there is any type of collision.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "CollisionRect.h"
#include "Logger.h"

CollisionRect::CollisionRect( const int x_, const int y_, const int w_, const int h_, const TypeCollision type_) :
	type(type_), rect{x_, y_, w_, h_}
{

}
		
CollisionRect::CollisionRect( const SDL_Rect rect_, const TypeCollision type_) : type(type_), rect(rect_)
{

}

CollisionRect::~CollisionRect()
{

}

/*
 * Method for checking the type collision.
 * @param strType_ : type collision 
 */
TypeCollision CollisionRect::stringToType( const std::string& strType_ )
{   
	//Control structure to check the type of collision
	if ( strType_ == "collision")
	{
		return COLLISION;
	}
	else if ( strType_ == "jump_through")
	{
		return JUMP_THROUGH;
	}
	else if ( strType_ == "level_end")
	{
		return LEVEL_END;
	}
	else if ( strType_ == "level_begin")
	{
		return LEVEL_BEGIN;
	}
	else if( strType_ == "level_end")
	{
		return LEVEL_END;
	}
	else if ( strType_ == "climb")
	{
		return CLIMB;
	}
	else if ( strType_ == "checkpoint")
	{
		return CHECKPOINT;
	}
	else if ( strType_ == "cutscene")
	{
		return CUTSCENE;
	} else {
		  Log(ERROR) << "Unknown collision type for " << strType_ << " .";
		  return NONE;
	  }
}
