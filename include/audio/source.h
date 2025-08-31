#pragma once
#include <AL/al.h>
#include <glm/vec3.hpp>
class Source 
{
    private:
        ALuint sourceID;
    public:
        Source();
        void setVolume(float volume);
        void setVelocity(glm::vec3 velocity);
        void setPitch(float pitch);
        void setPosition(glm::vec3 Position);
        void setLooping(bool shouldLoop);
        void play(ALuint buffer);
        bool isPlaying();
        void pause();
        void continuePlaying();
        void stop();
        void deleteSource();

};

