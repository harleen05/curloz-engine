#include "global/global.h"
#include "global/globalCamera.h"
#include "global/globalConfig.h"
#include "global/globalStatus.h"


void global::init()
{
        global::Status::init();
        global::Config::init();
        global::CameraManager::init();
}

void global::update()
{
        global::CameraManager::update();
}

void global::saveConfig()
{
        global::CameraManager::saveConfig();
}
