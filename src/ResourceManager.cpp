/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @BStateMagicProtectile.cpp
* General resource management class.
*/

#include "ResourceManager.h"
#include "Logger.h"

/**
* The constructor.
* Initializes attributes.
*/
ResourceManager::ResourceManager() :
  resources()
{

}

/**
* The destructor.
*/
ResourceManager::~ResourceManager()
{
  std::map<std::string, Sprite::SpritePtr>::const_iterator it;
  
  for ( it = this -> resources.begin(); it != this -> resources.end(); it++ )
  {
    if ( it -> second.use_count() != 1 )
    {
        Log(WARN) << "Resource deleted with use count different than 1 (" << it -> first << ").";
    }
  }
}

/**
* Retrieves the sprite from the resources.
* @return The pointer to the wanted sprite. Creates it, if not already in resources.
* @param PATH : Path to the desired sprite.
*/
Sprite* ResourceManager::get( const std::string& PATH )
{
  std::map<std::string, Sprite::SpritePtr>::const_iterator it;
  it = this -> resources.find( PATH );

  if ( it != this -> resources.end() )
  {
    return it -> second.get();
  }
  
  else
  {
    return load( PATH ).get();
  }
}

/**
* Registers the resource on the resources map.
* @param PATH : Path to the sprite. Will be the key value.
* @param resource_ : The shared pointer for the resource.
*/
void ResourceManager::registerResource( const std::string& PATH, Sprite::SpritePtr resource_ )
{
  this -> resources.insert( std::make_pair(PATH, resource_) );
}

/**
* Unregisters the resource on the resources map.
* @param PATH : The key (path to sprite) on the map.
*/
void ResourceManager::unregisterResource( const std::string& PATH )
{
  std::map<std::string, Sprite::SpritePtr>::const_iterator it;
  it = this -> resources.find( PATH );

  if (it != this -> resources.end()){
    this -> resources.erase( it );
  }
}

/**
* Constructs the desired sprite.
* @param PATH : Path to the desired sprite.
*/
Sprite::SpritePtr ResourceManager::load( const std::string& PATH )
{
  Sprite::SpritePtr sprite = std::make_shared<Sprite>( PATH );
  registerResource( PATH, sprite );
  return sprite;
}
