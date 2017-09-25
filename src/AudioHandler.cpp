/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @AudioHandler.cpp
* The audio handler.
* Class that handles all the audio in the game. Is a singleton, so should be called in each
* 	state.
* License: Copyright (C) 2014 Alke Games.*/

#include "AudioHandler.h"
#include "Game.h"
#include "Logger.h"

/**
* The constructor.
* Initializes all the attributes.
*/
AudioHandler::AudioHandler() :
	currentMusic( nullptr ),
	currentEffects()
{

}

/**
* The destructor.
* Frees the allocated attributes.
*/
AudioHandler::~AudioHandler()
{
	// Unload the current music.
	if ( this -> currentMusic != nullptr )
	{
		Mix_FreeMusic( this -> currentMusic );
		this -> currentMusic = nullptr;
	}

	// Log( DEBUG ) << "Still had " << this->currentEffects.size() << " sfx on vector.";

	// Clearing effects.
	for ( auto sfx : this -> currentEffects )
	{
		Mix_FreeChunk( sfx.effect );
	}

	this->currentEffects.clear();
}

/**
* Sets the current music.
* If one already exists, frees it first.
* @param path_ : The path to the desired music.
*/
void AudioHandler::setCurrentMusic( const std::string &path_ )
{
	if ( this -> currentMusic != nullptr )
	{
		Mix_FreeMusic( this -> currentMusic );
		this -> currentMusic = nullptr;
	}

	this -> currentMusic = Mix_LoadMUS( path_.c_str() );
}

/**
* Plays the current music.
* @note Will warn if there is no music loaded.
* @param times_ : Times to loop the song. MIX_LOOP (or -1) for infinite looping.
*/
void AudioHandler::playMusic( const int times_ )
{
	if ( this -> currentMusic )
	{
		Mix_PlayMusic( this -> currentMusic, times_ );
	}
	else{
		Log( WARN ) << "There is no song loaded.";
	}
}

/**
* Stops playing the current music.
*/
void AudioHandler::stopMusic()
{
	Mix_HaltMusic();
}

/**
* Sets the volume for the music.
* If the percent_ is over 100, will be set to 100.
* @param percent_ : The volume percentage (0-100).
*/
void AudioHandler::setMusicVolume( const unsigned int percent_ )
{
	const int value = percent_ * MIX_MAX_VOLUME/100;
	Mix_VolumeMusic( value );
}

/**
* Sets the current effect.
* If one already exists, frees it first.
* @param path_ : The path to the desired effect.
*/
void AudioHandler::addSoundEffect( const std::string& path_ )
{
	Mix_Chunk *effect = Mix_LoadWAV( path_.c_str() ); // Sound effect.
	SoundEffect sfx = {effect, -1};

	if ( effect == nullptr )
	{
		Log( DEBUG ) << "Loaded null chunk " << path_ << " " << Mix_GetError();
	}

	/// @todo Resource manager for audio.
	this->currentEffects.push_back( sfx );

	playEffect( 0 );
}

/**
* Plays the current effect.
* @note Will warn if there is no effect loaded.
* @param times_ : Times to loop the song. MIX_LOOP (or -1) for infinite looping.
*/
void AudioHandler::playEffect( const int times_ )
{
	const int playedChannel = Mix_PlayChannel( -1, 
			this -> currentEffects.back().effect, times_ ); // Channel played with effects

	if ( playedChannel == -1 )
	{
		Log( ERROR ) << "Failed to play sound effect on channel " << playedChannel << ". " << Mix_GetError();
	}

	this -> currentEffects.back().channel = playedChannel;

	Mix_ChannelFinished( AudioHandler::channelDone );
}

/**
* Sets the volume for the effects.
* If the percent_ is over 100, will be set to 100.
* @param percent_ : The volume percentage (0-100).
*/
void AudioHandler::setEffectVolume( const unsigned int percent_ )
{
	const int value = percent_ * MIX_MAX_VOLUME/100;
	Mix_Volume( -1, value );
}

/**
* Changes current music.
* Stops the music, sets it, and plays it with infinite looping.
* @param path_ : The path to the desired music.
*/
void AudioHandler::changeMusic( const std::string& path_ )
{
	stopMusic();
	setCurrentMusic( path_ );
	playMusic( MIX_LOOP );
}

/**
* Clear the current channel.
* @param channel_ : The number of the channel.
*/
void AudioHandler::clearChannel( const int channel_ )
{
	std::vector< SoundEffect >::iterator it;

	// Clearing all the effetcs played in the channel.
	for ( it = this -> currentEffects.begin(); it != this -> currentEffects.end(); )
	{
		if ( it -> channel == channel_ )
		{
			Mix_FreeChunk( it -> effect );
			it -> effect = nullptr;
			this -> currentEffects.erase( it );
		} else
		{
			it++;
		}
	}
}

/**
* Finish the channel
* @param channel_ : The number of the channel.
*/
void AudioHandler::channelDone( int channel_ )
{
	// Log( DEBUG ) << "Channel [" << channel_ << "] done. ( CALLBACK )";

	Game::instance().getAudioHandler().clearChannel( channel_ );
}
