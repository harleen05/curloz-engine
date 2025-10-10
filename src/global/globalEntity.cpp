#include "global/globalEntity.h"
#include "global/globalConfig.h"

std::string global::Entity::getConfigFile()
{
        auto config = global::Config::getConfig();
        return config["entity"]["configfile"].value_or("assets/config/entity.json");
}
std::string global::Entity::getVertexShader()
{
        auto config = global::Config::getConfig();
        return config["entity"]["vertexshaderfile"].value_or("assets/shaders/shaderEntity.vert");
}
std::string global::Entity::getFragmentShader()
{
        auto config = global::Config::getConfig();
        return config["entity"]["fragmentshaderfile"].value_or("assets/shaders/shaderEntity.frag");
}

        


