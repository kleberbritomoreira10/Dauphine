/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Math.cpp
* The math operations of the game.
* Class that do the math operations in the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "Math.h"

/**
* Sets the value of the numbers in zero, positives or negatives.
* Is used for sets the character state of life.
* @param number_: The value that will be parameterized.
*/
int Math::sign( const int number_ )
{
  if( number_ > 0 )
  { 
    return 1;
  }
  else if( number_ < 0 )
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
* Sets the value of the numbers in zero, positives or negatives.
* Is used for sets the character state of life.
* @param number_: The value that will be parameterized.
*/
int Math::sign( const double number_ )
{
  if( number_ > 0.0 )
  { 
    return 1;
  }
  else if( number_ < 0.0 )
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
