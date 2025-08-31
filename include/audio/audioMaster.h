#pragma once
#include "dr_wav.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "global/general.h"
#include <iostream>
#include <string>

class AudioMaster
{
    private:
        static ALCdevice *device;
        static ALCcontext *context;

    public:
        static void init();
        static void cleanUp();
        static ALuint loadSound(std::string filePath);
        static void update();
        static void setListenerData();
};






