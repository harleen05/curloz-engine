#include "global/globalConfig.h"
#include "toml/toml.hpp"

toml::table global::Config::config;

void global::Config::init()
{
        config = toml::parse_file("assets/config/global.toml");
}

toml::table &global::Config::getConfig()
{
       return config; 
}
