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
 * @param file_name : Path to the desired script, i.e. "lua/level1/Player.lua".
 */
LuaScript::LuaScript( const std::string& file_name) 
{
  // @todo Log an error message for different lua error codes.
  this -> level = 0;
  this -> lua_state = luaL_newstate();

  const int LOADED_FILE = luaL_loadfile( this -> lua_state, file_name.c_str());
  const int CALLED_FUNCTION = lua_pcall(lua_state, 0, 0, 0);
  
  //Struct control to open libs. Otherwise load file
  if ( LOADED_FILE == LUA_OK && CALLED_FUNCTION == LUA_OK ) 
  {
    if ( this -> lua_state != nullptr )
    {
      luaL_openlibs( this -> lua_state );
    }
  } else {
      Log(DEBUG) << "Failed to load (" << file_name << ")";
      this -> lua_state = nullptr;
    }
}

/*
 * The destructor.
 * Closes the lua state, if open.
 */
LuaScript::~LuaScript() 
{
  if ( this -> lua_state != nullptr )
  {
    lua_close( this -> lua_state );
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
  if ( lua_isnil( this -> lua_state, -1) ) 
  {
    return std::vector<int>();
  }
  
  lua_pushnil( this -> lua_state );
  while( lua_next( this -> lua_state, -2) ) 
  {
    v.push_back((int)lua_tonumber( this -> lua_state, -1 ));
    lua_pop( this -> lua_state, 1 );
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
  luaL_loadstring( this -> lua_state, code.c_str()); 
  //Execute function
  lua_pcall( this -> lua_state, 0, 0, 0);
  //Get function
  lua_get_global( this -> lua_state, "getKeys"); 
  lua_pushstring( this -> lua_state, name_.c_str());
  //Execute function
  lua_pcall( this -> lua_state, 1 , 1, 0 ); 
  
  //Declare constant test and convert to string the lua_state
  const std::string test = lua_tostring( lua_state, -1 );
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
 * Checks where the 'variable_name' variable exists inside the lua script.
 * @param variable_name : The varaible you want to get a value from.
 */
bool LuaScript::unlua_getToStack( const std::string& variable_name ) 
{
  this -> level = 0;
  std::string var = "";

  //Structure repetition to check whether the name is set or not
  for ( unsigned int i = 0; i < variable_name.size(); i++ ) 
  {
    if ( variable_name.at(i) == '.') 
    {
      if ( this -> level == 0) 
      {
        lua_get_global( this -> lua_state, var.c_str() );
      } else {
          lua_get_field( this -> lua_state, -1, var.c_str() );
        }
      if ( lua_isnil( this -> lua_state, -1)) 
      {
        Log(ERROR) << "Can't get " << variable_name << ". " << var << " is not defined.";
        return false;
      } else {
          var = "";
          this -> level++;
        }
    } else {
        var += variable_name.at(i);
      }
  }
  if ( level == 0 ) 
  {
    lua_get_global( this -> lua_state, var.c_str() );
  } else {
      lua_get_field( this -> lua_state, -1, var.c_str() );
    }
  if ( lua_isnil(lua_state, -1) ) 
  {
    Log(ERROR) << "Can't get " << variable_name << ". " << var << " is not defined.";
    return false;
  }
  return true;
}

