#pragma once 

#include "global.h"

namespace global 
{
        class Display
        {
                public:
                        static int getWidth();
                        static int getheight();
                        static bool VSync();
                        static int getRefreshRate();

                        static void saveConfig();
        };
};
