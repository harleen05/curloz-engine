#pragma once

#include<vector>
#include <memory>
#include <string>
#include "entity/renderStatic.h"

class Entity
{
        public:
                static void init();
                static std::string entityConfigFile;
                static std::vector<std::unique_ptr<renderStatic>> p_renderStatic;
                static void loadEntities();

                static void saveConfig();
                

                static void update (float deltaTime);

};
