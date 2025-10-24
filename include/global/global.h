#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include "toml/toml.hpp"
#include "camera/camera.h"
#include "globalEntity.h"

/**
 *Global namespace
 */

namespace global 
{
        void preInit();
        /**
         * @note
         * Should be called before window creation because
         * window resolution is required and is initialized by
         * global config
         *
         * @brief
         * Pre initialization calls init functions
         * of those globals that dont require calling opengl functions
         * Currently ->
         * 1. global status
         * 2. global config files;
         *
         */


        void postInit();
        /**
         * @note
         * Post initialization must be called after 
         * glad has initialized opengl functions
         *
         * @brief
         * Post initialization call init functions
         * of those global that require opengl function
         * Currently ->
         * 1. global camera
         *
         */

        void update();
        /**
         * @brief
         * Calls the update functions of other global variables
         * Right now there are->
         * 1. global::cameraManager 
         *      @remember -> need to seperately call the processKeyboard function
         *
         * 2.
         *
         */

        void saveConfig();
        /**
         * @note 
         * must be called right after the program exits
         * the main game loop
         *
         * @brief
         * writes to respective config file
         * Right now there are->
         * 1. global::cameraManager
         *
         */

};


