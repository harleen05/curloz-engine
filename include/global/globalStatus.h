#pragma once

#include "global/global.h"

namespace global 
{
        /// Which release model
        enum PROFILE 
        {
               RELEASE_MODE,
               DEBUG_MODE
        };

        /// Which mode currently it is
        enum MODE
        {
                GAME_MODE,
                EDIT_MODE
        };

        class Status
        {
                public:
                        static void init();
                        /**
                         * @note
                         * should come in preInit()
                         *
                         * @brief
                         * Very important
                         * status is determined by how engine is compiled
                         * if profile is debug, then PROFILE is set to DEBUG_MODE
                         * which by defaults start engine in EDIT MODE
                         *
                         * @detail
                         * The engine is in its pre-state, 
                         * so by default when making game, the engine's editing stuff
                         * like editing menus, editors n stuff would only be enabled when 
                         * engine is compiled in debug mode.
                         * Once compiled with Release model
                         * No menus will be shown
                         * Basically it would be the game XD
                         *
                         */

                        static PROFILE Profile;
                        static MODE Mode;
        };

}
