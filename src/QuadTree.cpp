/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @QuadTree.cpp
* The control of the trees in the game.
* Class that controll the tree movements of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "QuadTree.h"
#include <assert.h>

#define MAX_NUMBER_OF_NODES 4
#define INDEX_NOT_ALLOWED -1
#define FIRST_NODE 0

/*
* Thanks to:
* http://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374
*/

/**
* The constructor.
* Initializes the attributes.
*/
QuadTree::QuadTree( int level_, SDL_Rect bounds_ ) :
	max_number_of_quadtree_objects( 10 ),
	max_number_of_quadtree_levels( 5 ),
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

	for( int i = 0; i < MAX_NUMBER_OF_NODES; i++ )
	{
		if( this -> nodes[ i ] != nullptr)
		{
			this -> nodes[ i ] -> clear();
			this -> nodes[ i ] = nullptr;

		}else
		{
			// No action.
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
	const int position_x = this -> bounds.x;
	const int position_y = this -> bounds.y;

	SDL_Rect rect0 = { position_x + subWidth, position_y, subWidth, subHeight };
	SDL_Rect rect1 = { position_x, position_y, subWidth, subHeight };
	SDL_Rect rect2 = { position_x, position_y + subHeight, subWidth, subHeight };
	SDL_Rect rect3 = { position_x + subWidth, position_y + subHeight, subWidth, subHeight };

	this -> nodes[ 0 ] = new QuadTree( this -> level + 1, rect0 );
	this -> nodes[ 1 ] = new QuadTree( this -> level + 1, rect1 );
	this -> nodes[ 2 ] = new QuadTree(this -> level + 1, rect2 );
	this -> nodes[ 3 ] = new QuadTree(this -> level + 1, rect3 );
}

/**
* Getting the index for objects of the Quad Tree on the game.
* @param rect_: object used for manage the postions in the axys.
*/
int QuadTree::getIndex( SDL_Rect rect_ )
{

	// int index = -1;
	int result_index = -1;
	double vertical_mid_point = this -> bounds.x + ( this -> bounds.w / 2 );
	double horizontal_mid_point = this -> bounds.y + ( this -> bounds.h / 2 );

	assert( vertical_mid_point > 0 && horizontal_mid_point > 0 );

	// Object can completely fit within the top quadrants
	bool top_quadrant = ( rect_.y < horizontal_mid_point && rect_.y + rect_.h < horizontal_mid_point );

	// Object can completely fit within the bottom quadrants
	bool bottom_quadrant = ( rect_.y > horizontal_mid_point );

	// Object can completely fit within the left quadrants
	if( rect_.x < vertical_mid_point && rect_.x + rect_.w < vertical_mid_point )
	{
		result_index = handle_index_left_quadrants( top_quadrant, bottom_quadrant );

	}else if ( rect_.x > vertical_mid_point ) // Object can completely fit within the right quadrants
	{
		result_index = handle_index_right_quadrants( top_quadrant, bottom_quadrant );

	}else
	{
		// No action.
	}

	return result_index;
}

int QuadTree::handle_index_left_quadrants( bool top_quadrant, bool bottom_quadrant )
{
	int index = -1;

	if( top_quadrant )
	{
		return index = 1;

	}else if ( bottom_quadrant )
	{
		return index = 2;

	}else
	{
		return index;
	}
}

int QuadTree::handle_index_right_quadrants( bool top_quadrant, bool bottom_quadrant )
{
	int index = -1;

	if ( top_quadrant )
	{
		return index = 0;
	}
	else if ( bottom_quadrant )
	{
		return index = 3;

	}else
	{
		return index;
	}
}

/**
* Managing the insertion for colisions objects of the Quad Tree on the game.
* @param rect_: object used for manage the positions in the axys.
*/
void QuadTree::insert( CollisionRect rect_ ){

	if( nodes[ 0 ] != nullptr )
	{
		int index = getIndex( rect_.rect );

		if( index != INDEX_NOT_ALLOWED )
		{
			nodes[ index ] -> insert( rect_ );
			return;

		}else
		{
			// No action.
		}

	}else
	{
		// No action.
	}

	this -> objects.push_back( rect_ );

	if( (int)this -> objects.size() > this -> max_number_of_quadtree_objects &&
		level < this -> max_number_of_quadtree_objects )
	{
		if( nodes[ FIRST_NODE ] == nullptr )
		{
			split();

		}else
		{
			// No action.
		}

		insert_all_objects();

	}else
	{
		// No action.
	}
}

void QuadTree::insert_all_objects()
{
	int i = 0;
	while( i < ( int ) this -> objects.size())
	{
		int index = getIndex( this -> objects.at( i ).rect );
		if( index != INDEX_NOT_ALLOWED )
		{
			CollisionRect move_rect = this -> objects.at( i );
			this -> objects.erase( this -> objects.begin() + i );
			nodes[ index ] -> insert( move_rect );

		}else
		{
			i++;
		}
	}
}

std::vector<CollisionRect> QuadTree::retrieve( std::vector< CollisionRect > &returnObjects_, SDL_Rect rect_ )
{

	int index = getIndex( rect_ );

	if( index != INDEX_NOT_ALLOWED && nodes[ FIRST_NODE ] != nullptr )
	{
		nodes[ index ] -> retrieve( returnObjects_, rect_ );

	}else
	{
		//No action.
	}

	returnObjects_.insert( returnObjects_.end(), this -> objects.begin(), this -> objects.end() );

	return returnObjects_;
}

/**
* Updating the objects in the game after the colisions.
* @param object_: object used for manage after the colisions.
*/
void QuadTree::setObjects( std::vector< CollisionRect > &objects_ )
{
	this -> objects.clear();
	this -> objects = objects_;
}
