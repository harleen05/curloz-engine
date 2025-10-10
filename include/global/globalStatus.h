#pragma once

#include "global/global.h"

namespace global 
{
        enum PROFILE 
        {
               RELEASE_MODE,
               DEBUG_MODE
        };

        enum MODE
        {
                GAME_MODE,
                EDIT_MODE
        };

        class Status
        {
                public:
                        static PROFILE Profile;
                        static MODE Mode;

                        static void init();

        };

}
