/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @QuadTree.cpp
* The control of the trees in the game.
* Class that controll the tree movements of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "QuadTree.h"

/*
* Thanks to:
* http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
*/

/**
* The constructor.
* Initializes the attributes.
*/
QuadTree::QuadTree( int level_, SDL_Rect bounds_ ) :
	maxObjects( 10 ),
	maxLevels( 5 ),
	level( level_ ),
	objects(),
	bounds( bounds_ ),
	nodes()
{
}

/**
* The destructor.
*/
QuadTree::~QuadTree()
{

}

/**
* Clearing the nodes objects of the Quad Tree on the game.
*/
void QuadTree::clear()
{
	this -> objects.clear();

	for( int i = 0; i < 4; i++ )
	{
		if( this -> nodes[ i ] != nullptr)
		{
			this -> nodes[ i ] -> clear();
			this -> nodes[ i ] = nullptr;
		}
	}
}

/**
* Applying the split in objects of the Quad Tree on the game.
*/
void QuadTree::split()
{
	const int subWidth = this -> bounds.w / 2;
	const int subHeight = this -> bounds.h / 2;
	const int x = this -> bounds.x;
	const int y = this -> bounds.y;

	SDL_Rect rect0 = { x + subWidth, y, subWidth, subHeight };
	SDL_Rect rect1 = { x, y, subWidth, subHeight };
	SDL_Rect rect2 = { x, y + subHeight, subWidth, subHeight };
	SDL_Rect rect3 = { x + subWidth, y + subHeight, subWidth, subHeight };

	this -> nodes[ 0 ] = new QuadTree( this -> level+1, rect0 );
	this -> nodes[ 1 ] = new QuadTree( this -> level+1, rect1 );
	this -> nodes[ 2 ] = new QuadTree(this -> level+1, rect2 );
	this -> nodes[ 3 ] = new QuadTree(this -> level+1, rect3 );
}

/**
* Getting the index for objects of the Quad Tree on the game.
* @param rect_: object used for manage the postions in the axys.
*/
int QuadTree::getIndex( SDL_Rect rect_ )
{
	int index = -1;
	double verticalMidpoint = this -> bounds.x + ( this -> bounds.w / 2 );
	double horizontalMidpoint = this -> bounds.y + ( this->bounds.h / 2 );

	// Object can completely fit within the top quadrants
	bool topQuadrant = ( rect_.y < horizontalMidpoint && rect_.y + rect_.h < horizontalMidpoint );
	// Object can completely fit within the bottom quadrants
	bool bottomQuadrant = ( rect_.y > horizontalMidpoint );

	// Object can completely fit within the left quadrants
	if( rect_.x < verticalMidpoint && rect_.x + rect_.w < verticalMidpoint )
	{
		if( topQuadrant )
		{
			index = 1;
		}
		else if ( bottomQuadrant )
		{
			index = 2;
		}
	}
	// Object can completely fit within the right quadrants
	else if ( rect_.x > verticalMidpoint )
	{
		if (topQuadrant)
		{
			index = 0;
		}
		else if ( bottomQuadrant )
		{
			index = 3;
		}
	}

	return index;
}

/**
* Managing the insertion for colisions objects of the Quad Tree on the game.
* @param rect_: object used for manage the postions in the axys.
*/
void QuadTree::insert( CollisionRect rect_ ){
	if( nodes[ 0 ] != nullptr )
	{
		int index = getIndex( rect_.rect );

		if( index != -1 )
		{
			nodes[ index ] -> insert( rect_ );
			return;
		}
	}

	this -> objects.push_back( rect_ );

	if((int)this -> objects.size() > this -> maxObjects && level < this->maxObjects )
	{
		if( nodes[ 0 ] == nullptr )
		{
			split();
		}

		int i = 0;
		while( i < ( int ) this -> objects.size())
		{
			int index = getIndex(this->objects.at(i).rect);
			if( index != -1 )
			{
				CollisionRect moveRect = this -> objects.at( i );
				this -> objects.erase( this -> objects.begin() + i );
				nodes[ index ] -> insert( moveRect );
			}
			else
			{
				i++;
			}
		}
	}
}

std::vector<CollisionRect> QuadTree::retrieve( std::vector<CollisionRect>& returnObjects_, SDL_Rect rect_ )
{
	int index = getIndex( rect_ );
	if( index != -1 && nodes[ 0 ] != nullptr )
	{
		nodes[ index ] -> retrieve( returnObjects_, rect_ );
	}

	returnObjects_.insert( returnObjects_.end(), this -> objects.begin(), this -> objects.end() );

	return returnObjects_;
}

/**
* Updating the objects in the game after the colisions.
* @param object_: object used for manage after the colisions.
*/
void QuadTree::setObjects( std::vector<CollisionRect>& objects_ )
{
	this -> objects.clear();
	this -> objects = objects_;
}
