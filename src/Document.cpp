/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Document.cpp
 * Subclass of Entity.
 * The objects of this class are entities, but have physics applied to them.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "Document.h"
#include "Logger.h"
#include "Game.h"

/*
@param x_ : position in x axis.
@param y_ : position in y axis.
*/
Document::Document( const double x_, const double y_, const std::string& path_, const std::string& pathDocumentText_) :
	Entity(x_, y_, path_ ), shouldRender(false), documentText(Game::instance().getResources().get(pathDocumentText_ ))
{
	if ( documentText == nullptr )
	{
		Log(ERROR) << "Could not load document text sprite.";
	}
}

Document::~Document ()
{

}

/*
Update the text sprite according to dt_ .
@param dt_ : Delta time. Time elapsed between one frame and the other.
*/
void Document::update ( const double dt_ )
{
	( ( void ) dt_ ); // Unused;
}

/*
Render the text sprite according to position x and y of the camera
@param cameraX_ : The x position of the camera.
@param cameraY_ : The y position of the camera.
*/
void Document::render ( const double cameraX_, const double cameraY_ )
{

	const double dx = this -> x - cameraX_;
	const double dy = this -> y - cameraY_;

	if ( this -> sprite != nullptr )
	{
		this -> sprite -> render ( dx, dy );
	}
	else
	{
		Log ( ERROR ) << "Failed to render document sprite.";
	}
}

/*
Render the document
Always renders on 0,0 position.
*/
void Document::renderDocumentText ()
{
	this -> documentText -> render ( 0, 0, nullptr, true );
}
