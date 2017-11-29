/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @Math.cpp
* The math operations of the game.
* Class that do the math operations in the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "Math.h"
#include <assert.h>
#include <cstddef>

#define NO_NUMBERS_TO_BE_PARAMETERIZED 0
#define INVALID_NUMBER -1
#define VALID_NUMBER 1

/**
* Sets the value of the numbers in zero, positives or negatives.
* Is used for sets the character state of life.
* @param number_to_be_parameterized: The value that will be parameterized.
*/
int Math::sign( const int number_to_be_parameterized )
{
  assert( !number_to_be_parameterized );

  if( number_to_be_parameterized > NO_NUMBERS_TO_BE_PARAMETERIZED )
  {
    return VALID_NUMBER;

  }else if( number_to_be_parameterized < NO_NUMBERS_TO_BE_PARAMETERIZED )
  {
    return INVALID_NUMBER;

  }else
  {
    return 0;
  }
}

/**
* Sets the value of the numbers in zero, positives or negatives.
* Is used for sets the character state of life.
* @param number_to_be_parameterized: The value that will be parameterized.
*/
int Math::sign( const double number_to_be_parameterized )
{

  if( number_to_be_parameterized > 0.0 )
  {
    return VALID_NUMBER;

  }else if( number_to_be_parameterized < 0.0 )
  {
    return INVALID_NUMBER;

  }else
  {
    return 0;
  }
}
