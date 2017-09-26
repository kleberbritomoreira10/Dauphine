/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @StateGame.cpp
* Parent class for other game states.
* License: Copyright (C) 2014 Alke Games.*/

#include "StateGame.h"

/**
* The destructor.
*/
StateGame::~StateGame()
{
	cleanEntities();
}

/**
* Adds an entity to the vector.
  @param entity: Reference to an Entity to be added.
*/
void StateGame::addEntity( Entity *const entity )
{
	this -> entities.push_back( entity );
}

/**
* Deletes all the entities inside the vector.
*/
void StateGame::cleanEntities()
{
	for ( auto entity : this -> entities )
	{
		delete entity;
		entity = nullptr;
	}

	this -> entities.clear();
}
