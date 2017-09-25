/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Document.h
 * Subclass of Entity.
 * The objects of this class are entities, but have physics applied to them.
 * License: Copyright (C) 2014 Alke Games.
 */

#ifndef INCLUDE_DOCUMENT_H
#define INCLUDE_DOCUMENT_H

#include "Entity.h"

class Document : public Entity
{

	public:
		/*
		@param x_ : position in x axis.
		@param y_ : position in y axis.
		*/
		Document ( const double x_, const double y_, const std::string &path_,
				 const std::string &pathDocumentText_ );

		virtual ~Document ();

		/*
		Update the text sprite according to dt_ .
		@param dt_ : Delta time. Time elapsed between one frame and the other.
		*/
		virtual void update ( const double dt_ );

		/*
		Render the text sprite according to position x and y of the camera
		@param cameraX_ : The x position of the camera.
		@param cameraY_ : The y position of the camera.
		*/
		virtual void render ( const double cameraX_, const double cameraY_ );

		void renderDocumentText ();

		bool shouldRender; // Bool variable to set when the Render of document should set

	private:
		Sprite *documentText; // A text sprite from a Document
};

#endif //INCLUDE_DOCUMENT_H
