#include <iostream>
#include "global/globalStatus.h"

using namespace global;

PROFILE Status::Profile;
MODE Status::Mode;

void Status::init()
{
        Profile = PROFILE::RELEASE_MODE;
        #ifdef DEBUG 
        Profile = PROFILE::DEBUG_MODE;
        #endif

        Mode = MODE::GAME_MODE;
        #ifdef DEBUG
        Mode = MODE::EDIT_MODE;
        #endif

        std::cout << "Release Status: " << (Profile == PROFILE::RELEASE_MODE) << std::endl;
        std::cout << "Game Status: " << (Mode == MODE::GAME_MODE) << std::endl;


}
