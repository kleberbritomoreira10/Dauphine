/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @GameSave.cpp
 * The game save.
 * Class that save the level in the game
*/

#include "GameSave.h"
#include "Logger.h"
#include <cstdlib>
#include <assert.h>
#include <cstddef>

#define PATH_SLOT_1 "saveSlot1.dauphine"
#define PATH_SLOT_2 "saveSlot2.dauphine"
#define PATH_SLOT_3 "saveSlot3.dauphine"

/**
 * The constructor.
 * Initializes all the attributes.
*/
GameSave::GameSave()
{

}

std::string filePath = "";

/**
 * Sets the slot for save game.
 * @param SAVE_SELECTION: The slot that will be allocated the save.
*/
void GameSave::setSlot ( int SAVE_SELECTION )
{	
	assert( SAVE_SELECTION >= 0 );
	switch ( SAVE_SELECTION )
  {	
		case this -> Selection::SLOT_1:
			this -> filePath = PATH_SLOT_1;
		break;

		case this -> Selection::SLOT_2:
			this -> filePath = PATH_SLOT_2;
		break;

		case this -> Selection::SLOT_3:
			this -> filePath = PATH_SLOT_3;
		break;
	}
}

/**
 * Creates the save file for the game.
 * If the save fails register in log a error message.
*/
void GameSave::createSave()
{	
	this -> saveFile.open ( this -> filePath.c_str() );	

	if ( !this -> saveFile.fail() )
  {
		this -> saveFile << "-1" << std::endl;
		this -> saveFile.close();
	} else
    {
	    Log(DEBUG) << "Could not create save file at " + this -> filePath;
    }

	return;
}

/**
 * Register the current level in a save file.
 * If the current level is one the registration should not be done.
 * @param level_: The number of the current level of the game.
 * @param player: The character of the game, in this method gets your position.
 * @param enemies: The enemies of the game.
 * @param SLOT: The slot where are saving the game.
*/
void GameSave::saveLevel ( unsigned int level_, Player* player, std::vector <Enemy*> enemies, unsigned SLOT )
{	
	assert(level_ > 0 );
	this -> setSlot(SLOT);	
	this -> saveFile.open( this -> filePath.c_str() );

	Log(DEBUG) << "Saved from level " << level_;
	Log(DEBUG) << "Saved on file " << this -> filePath;

	if ( !this -> saveFile.fail() )
  {
		this -> CURRENT_LEVEL = level_;
		this -> saveFile << this -> CURRENT_LEVEL << std::endl;
		this -> saveFile << player -> x << std::endl;
		this -> saveFile << player -> y << std::endl;
		this -> saveFile << enemies.size() << std::endl;
	
  	for ( auto enemy : enemies )
    {
			this -> saveFile << enemy -> isDead() << " ";
		}
		
		this -> saveFile.close();
	} else
    {
		  Log(DEBUG) << "Could not open save file at " + this -> filePath;
    }
}

/**
 * Getting the number of the current level.
 * @continueSelection_: The number of the slot where is saved the game.
*/
int GameSave::get_saved_level ( int continueSelection_ )
{	
	assert( continueSelection_ >= 0 );
	this -> saveSelection = continueSelection_;
	
	std::string level = "-1";

	if ( this -> saveSelection == 0 )
  {
		this -> continueFile.open( PATH_SLOT_1 );   
	} else
	  {
	    //Nothing to do	
	  }

	if ( this -> saveSelection == 1 )
  {
		this -> continueFile.open( PATH_SLOT_2 );
	} else
	  {
	    //Nothing to do	
	  }

	if ( this -> saveSelection == 2)
  {
		this -> continueFile.open ( PATH_SLOT_3 );
	} else
	  {
	    //Nothing to do	
	  }
	
	this -> continueFile >> level;

	this -> continueFile.close();

	return std::stoi(level);
}

/**
 * The analyzer if the game is saved.
 * @SAVE_SLOT: The slot where is saved the game.
*/
bool GameSave::is_saved( const int SAVE_SLOT )
{	
	assert( SAVE_SLOT >= 0 );
	this -> setSlot( SAVE_SLOT );	
	this -> continueFile.open( this -> filePath.c_str() );

	std::string testSave = ""; 

	this -> continueFile >> testSave;

	Log(DEBUG) << "TestSave " << testSave;
	this -> continueFile.close();

	if ( testSave == "-1" )
  {
		Log(WARN) << "There is NO save at slot " << 1 + SAVE_SLOT;
		return false;
	} else
    {
		  Log(WARN) << "There is a save at slot " << 1 + SAVE_SLOT;
		  this -> continueFile.close();
		  return true;
	  }
}

/**
 * Getting a position of a player in a game.
 * @playerX: The horizontal position of the player in the game.
 * @playerY: The vertical position of the player in the game.
 * @SLOT: The slot where the game will be saved.
*/
void GameSave::get_player_position ( double& player_x, double& player_y, const int SLOT )
{	
	assert( SLOT >= 0);
	setSlot( SLOT );
	this -> continueFile.open( filePath.c_str(), std::ios_base::in );
	this -> continueFile >> CURRENT_LEVEL; 
	this -> continueFile >> player_x;
	this -> continueFile >> player_y;
	this -> continueFile.close();	
}

/**
 * The analyzer if the enemy is dead.
 * @numEnemy: The quantity of enemies near of the character.
 * @SLOT: The slot where the game will be saved.
*/
bool GameSave::is_enemy_dead ( const int NUMBER_ENEMY, const int SLOT )
{	
	assert( SLOT >= 0);
	  
	setSlot( SLOT );

	double skip = 0; //Shifts right and adds either 0s, if value is an unsigned type, or extends the top bit (to preserve the sign) if its a signed type.
	this -> continueFile.open( filePath.c_str(), std::ios_base::in );
	this -> continueFile >> skip;
	this -> continueFile >> skip;
	this -> continueFile >> skip;
	
	int totalEnemies = 0; //Declaring variable to count quantity enemies
	this -> continueFile >> totalEnemies;

	int currentEnemy = 0; //Declaring variable to know current enemy
	bool rc = false;    //Declaring boolean variable to identify if enemy is dead 
	
	Log(DEBUG) << "Total Enemies on Level " << totalEnemies;
	for ( int i = 0; i < totalEnemies; i++ )
  {
		Log(DEBUG) << "Is Enemy " << i << " dead?";		
		this -> continueFile >> currentEnemy;

		Log(DEBUG) << "Enemy under test dead status: " << currentEnemy;		
		if ( i == NUMBER_ENEMY )
    {
			if ( currentEnemy == 1 )
      {
				rc = true;
      } else
	      {
	        //Nothing to do	
	      }
			break;			
		} else
	    {
	      //Nothing to do	
	    }
	}

	this -> continueFile.close();	

	/*if(rc)
	    Log(DEBUG) << "YES"; 
		else
	 	  Log(DEBUG) << "NO";
	*/		 	 
	
	return rc;
}
