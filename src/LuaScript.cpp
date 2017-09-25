/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LuaScript.cpp
 * Include the characteristic of the language lua. Run Script
 * License: Copyright (C) 2014 Alke Games.
 */

#include "LuaScript.h"
#include "Logger.h"

/*
 * The constructor.
 * Initializes a new lua state, and loads the desired script.
 * @param filename_ : Path to the desired script, i.e. "lua/level1/Player.lua".
 */
LuaScript::LuaScript( const std::string& filename_) 
{
  // @todo Log an error message for different lua error codes.
  this -> level = 0;
  this -> luaState = luaL_newstate();

  const int loadedFile = luaL_loadfile( this -> luaState, filename_.c_str());
  const int calledFunction = lua_pcall(luaState, 0, 0, 0);
  
  //Struct control to open libs. Otherwise load file
  if ( loadedFile == LUA_OK && calledFunction == LUA_OK ) 
  {
    if ( this -> luaState != nullptr )
    {
      luaL_openlibs( this -> luaState );
    }
  } else {
      Log(DEBUG) << "Failed to load (" << filename_ << ")";
      this -> luaState = nullptr;
    }
}

/*
 * The destructor.
 * Closes the lua state, if open.
 */
LuaScript::~LuaScript() 
{
  if ( this -> luaState != nullptr )
  {
    lua_close( this -> luaState );
  }
  this -> level = 0;
}

/*
 * Gets an int vector.
 * @param name_ : The table which contains the int vector. 
 */
std::vector<int> LuaScript::unlua_getIntVector( const std::string& name_ ) 
{ 
  //std was declared to use the vector in standard library
  std::vector<int> v;
  unlua_getToStack(name_);

  //If the array is not found
  if ( lua_isnil( this -> luaState, -1) ) 
  {
    return std::vector<int>();
  }
  
  lua_pushnil( this -> luaState );
  while( lua_next( this -> luaState, -2) ) 
  {
    v.push_back((int)lua_tonumber( this -> luaState, -1 ));
    lua_pop( this -> luaState, 1 );
  }
  
  //Method to clean  
  unlua_clean();
  return v;
}

/*
 * Gets the keys from a table.
 * @param name_ : The name of the table.
 */    
std::vector<std::string> LuaScript::unlua_getTableKeys( const std::string& name_) 
{
  // function for getting table keys
  std::string code =
  "function getKeys(name_) "
  "s = \"\""
  "for k, v in pairs(_G[name_]) do "
  "    s = s..k..\",\" "
  "    end "
  "return s "
  "end"; 
  
  //Execute code
  luaL_loadstring( this -> luaState, code.c_str()); 
  //Execute function
  lua_pcall( this -> luaState, 0, 0, 0);
  //Get function
  lua_getglobal( this -> luaState, "getKeys"); 
  lua_pushstring( this -> luaState, name_.c_str());
  //Execute function
  lua_pcall( this -> luaState, 1 , 1, 0 ); 
  
  //Declare constant test and convert to string the LuaState
  const std::string test = lua_tostring( luaState, -1 );
  std::vector<std::string> strings;
  std::string temp = "";

  Log(DEBUG) << "TEMP: " << test;
  
  //Repetition structure to check debug.
  for ( unsigned int i = 0; i < test.size(); i++ ) 
  {
    if ( test.at(i) != ',' ) 
    {
      temp += test.at(i);
    } else {
        strings.push_back(temp);
        temp= "";
      }
    }
  unlua_clean();
  return strings;
}

/*
 * Validates existance of the variable.
 * Checks where the 'variableName_' variable exists inside the lua script.
 * @param variableName_ : The varaible you want to get a value from.
 */
bool LuaScript::unlua_getToStack( const std::string& variableName_ ) 
{
  this -> level = 0;
  std::string var = "";

  //Structure repetition to check whether the name is set or not
  for ( unsigned int i = 0; i < variableName_.size(); i++ ) 
  {
    if ( variableName_.at(i) == '.') 
    {
      if ( this -> level == 0) 
      {
        lua_getglobal( this -> luaState, var.c_str() );
      } else {
          lua_getfield( this -> luaState, -1, var.c_str() );
        }
      if ( lua_isnil( this -> luaState, -1)) 
      {
        Log(ERROR) << "Can't get " << variableName_ << ". " << var << " is not defined.";
        return false;
      } else {
          var = "";
          this -> level++;
        }
    } else {
        var += variableName_.at(i);
      }
  }
  if ( level == 0 ) 
  {
    lua_getglobal( this -> luaState, var.c_str() );
  } else {
      lua_getfield( this -> luaState, -1, var.c_str() );
    }
  if ( lua_isnil(luaState, -1) ) 
  {
    Log(ERROR) << "Can't get " << variableName_ << ". " << var << " is not defined.";
    return false;
  }
  return true;
}

