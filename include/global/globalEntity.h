#pragma once

#include "global.h"

namespace global 
{
         
        class Entity 
        {
                public:
                        static std::string getConfigFile();
                        static std::string getVertexShader();
                        static std::string getFragmentShader();

        };
}
