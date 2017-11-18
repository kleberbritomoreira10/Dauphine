#include "Game.h"
#include "Logger.h"
#include "Configuration.h"

// Main function, called in the beggining of the game.
int main ( int argc, char** argv )
{
  // Unused.
  ( ( void )argc );
  ( ( void )argv );

  // Initialize the configuration values, such as width and height of the screen.
  Configuration::initialize();

  bool systemsInitialized = SDLWrapper::initialize();

  if ( systemsInitialized )
  {
		// Start the game.
    Game::instance().runGame();
		// Deallocate the instance of the game.
    delete &Game::instance();
  } else
    {
      //Nothing to do
    }

  // Closes SDL used libraries.
  SDLWrapper::close();

  return 0;
}
