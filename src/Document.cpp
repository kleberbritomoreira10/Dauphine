
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
Document::Document( const double x_, const double y_, const std::string& PATH, const std::string& pathDocumentText_) :
	Entity(x_, y_, PATH ), should_render(false), documentText(Game::instance().get_resources().get(pathDocumentText_ ))
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
Update the text sprite according to DELTA_TIME .
@param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void Document::update ( const double DELTA_TIME )
{
	( ( void ) DELTA_TIME ); // Unused;
}

/*
Render the text sprite according to position x and y of the camera
@param camera_position_x : The x position of the camera.
@param camera_position_y : The y position of the camera.
*/
void Document::render ( const double camera_position_x, const double camera_position_y )
{

	const double dx = this -> x - camera_position_x;
	const double dy = this -> y - camera_position_y;

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
void Document::render_document_text ()
{
	this -> documentText -> render ( 0, 0, nullptr, true );
}
