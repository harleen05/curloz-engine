#include "AL/al.h"
#include "audio/dr_wav.h"
#include "audio/audioMaster.h"

//Our buffer(audio data) class
ALCdevice *AudioMaster::device = NULL;
ALCcontext *AudioMaster::context = NULL;

//Initialize buffer
void AudioMaster::init()
{
    device = alcOpenDevice(nullptr); 
    if(!device) 
    {
        std::cout<<"Could not initialize sound source"<<std::endl;
        return;
    }
    context = alcCreateContext(device, nullptr);
    if(!context)
    {
        std::cout<<"Could not create audio context"<<std::endl;
        return;
    }
    alcMakeContextCurrent(context);

    alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);
    ALenum distModel = alGetInteger(AL_DISTANCE_MODEL);
    std::cout << "Distance model: " << distModel << " (AL_INVERSE_DISTANCE_CLAMPED = " << AL_INVERSE_DISTANCE_CLAMPED << ")" << std::endl;
}

//clean everything up
void AudioMaster::cleanUp()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

//load our game sound
ALuint AudioMaster::loadSound(std::string filePath)
{
    std::cout<<"opening sound path->"<<filePath<<std::endl;
    ALuint channels, sampleRate;
    drwav_uint64 totalFrames;

    drwav_int16 *pSampleData = drwav_open_file_and_read_pcm_frames_s16(filePath.c_str(), &channels, &sampleRate, &totalFrames, NULL);
    if(pSampleData == NULL)
    {
        std::cout<<"Could not parse file"<<std::endl;
        return -1;
    }

    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    ALsizei dataSize = totalFrames * channels * sizeof(drwav_int16);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pSampleData, dataSize, sampleRate);

    std::cout<<"Parsed sound file succesfully"<<std::endl;
    drwav_free(pSampleData, NULL);
    return buffer;

}

//sets listener data
void AudioMaster::setListenerData()
{
    alListener3f(AL_POSITION, generalInfo.camera.Position.x, generalInfo.camera.Position.y, generalInfo.camera.Position.z);
    alListener3f(AL_VELOCITY, 0.0f,0.0f,0.0f);
    
    ALfloat listenerOrientation[] = 
    {
        generalInfo.camera.Front.x, generalInfo.camera.Front.y, generalInfo.camera.Front.z, //the up vector
        generalInfo.camera.Up.x, generalInfo.camera.Up.y, generalInfo.camera.Up.z
    };
    alListenerfv(AL_ORIENTATION, listenerOrientation);
}

//update listener position and orientation
void AudioMaster::update()
{
    alListener3f(AL_POSITION, generalInfo.camera.Position.x, generalInfo.camera.Position.y, generalInfo.camera.Position.z);
    ALfloat listenerOrientation[] = 
    {
        generalInfo.camera.Front.x, generalInfo.camera.Front.y, generalInfo.camera.Front.z, //the front vector of our listener
        generalInfo.camera.Up.x, generalInfo.camera.Up.y, generalInfo.camera.Up.z //the up vector of our listener
    };
    alListenerfv(AL_ORIENTATION, listenerOrientation);

}
