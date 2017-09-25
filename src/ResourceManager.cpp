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
* @param path_ : Path to the desired sprite.
*/
Sprite* ResourceManager::get( const std::string& path_ )
{
  std::map<std::string, Sprite::SpritePtr>::const_iterator it;
  it = this -> resources.find( path_ );

  if ( it != this -> resources.end() )
  {
    return it -> second.get();
  }
  
  else
  {
    return load( path_ ).get();
  }
}

/**
* Registers the resource on the resources map.
* @param path_ : Path to the sprite. Will be the key value.
* @param resource_ : The shared pointer for the resource.
*/
void ResourceManager::registerResource( const std::string& path_, Sprite::SpritePtr resource_ )
{
  this -> resources.insert( std::make_pair(path_, resource_) );
}

/**
* Unregisters the resource on the resources map.
* @param path_ : The key (path to sprite) on the map.
*/
void ResourceManager::unregisterResource( const std::string& path_ )
{
  std::map<std::string, Sprite::SpritePtr>::const_iterator it;
  it = this -> resources.find( path_ );

  if (it != this -> resources.end()){
    this -> resources.erase( it );
  }
}

/**
* Constructs the desired sprite.
* @param path_ : Path to the desired sprite.
*/
Sprite::SpritePtr ResourceManager::load( const std::string& path_ )
{
  Sprite::SpritePtr sprite = std::make_shared<Sprite>( path_ );
  registerResource( path_, sprite );
  return sprite;
}
