#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include "toml/toml.hpp"
#include "camera/camera.h"
#include "globalEntity.h"


namespace global 
{
        void init();

        void update();

        void saveConfig();
};


