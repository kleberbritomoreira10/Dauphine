/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @Logger.cpp
 * File responsible for displaying the progress of the game on the console
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "Logger.h"

Logger::Logger()
{

}

/*
 * Method to print and clean the logger
 */
Logger::~Logger()
{
  this -> os << std::endl;
  fprintf(stderr, "%s", this -> os.str().c_str());
  fflush(stderr);
}

/*
 * Method to display the logger
 * @param level_ : level to string
 */
std::ostringstream& Logger::log( const LogLevel level_)
{
  assert(level_ != nullptr);  
  this -> os << levelToString(level_);
  return this -> os;
}

/*
 * Display in console the information about game
 * @param level_ : level to string
 */
std::string Logger::levelToString( const LogLevel level_)
{
  assert(level_ != nullptr);
  static const char* const buffer[] = {"[Info]:    ", "[Warning]: ", "[Error]:  ", "[Debug]:   "};
  return buffer[level_];
}
