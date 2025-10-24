#pragma once

#include "global.h"
#include "toml/toml.hpp"

namespace global 
{
        class Config 
        {
                private:
                        /// main global configuration file (assets/config/global.toml)
                        static toml::table config;
                public:
                        static void init();
                        /**
                         * @note
                         * must be called in preInit()
                         *
                         * @brief
                         * This function parses the global configuration file
                         *
                         */

                        static toml::table &getConfig();
                        /**
                         * @brief
                         * called whenever the game attributes are required
                         *
                         */

        };

}
