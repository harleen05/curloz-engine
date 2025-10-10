#pragma once 

#include <algorithm>
#include <memory>
#include "lightingDirection.h"
#include "lightingPoint.h"
#include "json/json.hpp"
#include <vector>



class Lighting 
{
        private:
                static nlohmann::json config;
                static std::string configFile;


        public:
                static lightingDirectional global;
                static glm::vec3 getGlobalLight_Direction();
                static void setGlobalLight_Direction(glm::vec3 Direction);
                static glm::vec3 *getGlobalLight_Color();
                static void setGlobalLightColor(glm::vec3 Color);


                static std::vector<std::unique_ptr<LightingPoint>> m_point_lights;
                static int getNumPointLights();


                static void init();
                static void update();
                static void saveConfig();

};
