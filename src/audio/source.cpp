#include "audio/source.h"
#include "AL/al.h"
#include <iostream>

/*our source(audio origin) data class */

//Initializes our source
Source::Source()
{
    alGenSources(1, &sourceID);
    alSourcef(sourceID, AL_ROLLOFF_FACTOR, ALfloat(1.2));
    alSourcef(sourceID, AL_REFERENCE_DISTANCE, ALfloat(5));
    alSourcef(sourceID, AL_MAX_DISTANCE, ALfloat(50));
}
//sets its velocity
void Source::setVelocity(glm::vec3 velocity)
{
    alSource3f(sourceID, AL_VELOCITY, velocity.x ,velocity.y, velocity.z);
}
//should looping be enabled?
void Source::setLooping(bool shouldLoop)
{
    alSourcei(sourceID, AL_LOOPING, shouldLoop ? AL_TRUE : AL_FALSE);
}
//set the volume
void Source::setVolume(float volume)
{
    alSourcef(sourceID, AL_GAIN, volume);
}
//set the pitch
void Source::setPitch(float pitch)
{
    alSourcef(sourceID, AL_PITCH, pitch);
}
//set the position
void Source::setPosition(glm::vec3 Position)
{
    alSource3f(sourceID, AL_POSITION, Position.x ,Position.y, Position.z);
}

//plays the sound
void Source::play(ALuint buffer)
{
    stop();
    alSourcei(sourceID, AL_BUFFER, buffer);
    alSourcePlay(sourceID);

    ALfloat rolloff, refDist, maxDist;
    alGetSourcef(sourceID, AL_ROLLOFF_FACTOR, &rolloff);
    alGetSourcef(sourceID, AL_REFERENCE_DISTANCE, &refDist);
    alGetSourcef(sourceID, AL_MAX_DISTANCE, &maxDist);
    std::cout << "FINAL - Rolloff: " << rolloff << ", RefDist: " << refDist << ", MaxDist: " << maxDist << std::endl;
}
//returns true if our source is radiating audio atm
bool Source::isPlaying()
{
    ALenum is_playing;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &is_playing);
    return is_playing == AL_PLAYING;
}
//pauses our audio
void Source::pause()
{
    alSourcePause(sourceID);
}
//continue our sound
void Source::continuePlaying()
{
    alSourcePlay(sourceID);
}
//stop the sound
void Source::stop()
{
    alSourceStop(sourceID);
}
//deletes the source
void Source::deleteSource()
{
    stop();
    alDeleteSources(1, &sourceID);
}

