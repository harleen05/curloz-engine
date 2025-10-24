#pragma once 

#include "global.h"

namespace global 
{
        /// Does not require initialization
        class Display
        {
                public:
                        static int getWidth();
                        /**
                         * @brief
                         * reads and return display[width]
                         * from global config file
                         *
                         */

                        static int getheight();
                        /**
                         * @brief
                         * reads and return display[height] 
                         * from global config file
                         *
                         */

                        static bool VSync();
                        /**
                         * @brief
                         * reads and returns whether display[vsync] is set to true or false
                         * from global config file
                         *
                         */

                        static int getRefreshRate();
                        /**
                         * @brief
                         * reads and return display[refreshrate] 
                         * from global config file
                         *
                         */


                        static void saveConfig();
                        /**
                         * work in progress
                         */

        };
};
