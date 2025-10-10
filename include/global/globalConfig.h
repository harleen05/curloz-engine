#pragma once

#include "global.h"
#include "toml/toml.hpp"

namespace global 
{
        class Config 
        {
                private:
                        static toml::table config;
                public:
                        static void init();
                        static toml::table &getConfig();

        };

}
