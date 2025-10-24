#pragma once

#include "global.h"

namespace global 
{
        /// Does not require initialization 
        class Entity 
        {
                public:
                        static std::string getConfigFile();
                        /**
                         * @brief
                         * takes a reference to global config file
                         * returns string of path to 
                         * entities file (assets/config/entity.json)
                         *
                         */

                        static std::string getVertexShader();
                        /*
                         * @brief
                         * same as getConfigFile,
                         * but returns path to vertex shader
                         *
                         */
                        static std::string getFragmentShader();
                        /*
                         * @brief
                         * same as getConfigFile,
                         * but returns path to fragment shader
                         *
                         */

        };
}
