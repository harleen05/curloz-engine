#include "global/global.h"
#include "global/globalCamera.h"
#include "global/globalConfig.h"
#include "global/globalStatus.h"


void global::preInit()
{
        global::Status::init();
        global::Config::init();
}
void global::postInit()
{
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
