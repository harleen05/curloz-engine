/*
 * This file handles our global camera
 */
#pragma once

#include <memory>
#include "camera/camera.h"

namespace global 
{
        class CameraManager
        {
                public:
                        /// Main camera pointer
                        static std::unique_ptr<Camera> camera;

                        /// The camera'a near and far values
                        static float m_Near;
                        static float m_Far;

                        /// UBO holding projection and view matrix
                        static GLuint UBO_MATRICES;
                        static glm::mat4 projection;
                        static glm::mat4 view;


                        static void init();
                        /**
                         * @brief
                         * the camera Manager initialization function 
                         * reads the [camera] index from global.toml file 
                         * and initializes data
                         *
                         * @detail
                         * After parsing the global config file
                         * It sets the following data :
                         * 1. Camera Position (should be stored as [camera][position])
                         * 2. Camera Front (should be stored as [camera][front])
                         * 3. Near and Far values of view frustum
                         *
                         * After setting those,
                         * it initializes a new camera unique_pointer using std::make
                         * Then initializes the global projection and view matrices respectively
                         *
                         * In the end it makes a Uniform Buffer Object 
                         * holding the Projection and view matrices "RESPECTIVELY" (important)
                         * and sets its binding point to '0'
                         *
                         */



                        static void update();
                        /**
                         * @note
                         * also need to seperately call the processKeyboard function
                         * @brief
                         * this function is/should be updated every frame
                         * first sets the camera view matrix from the camera pointer
                         * then checks if fov had changed, if yes then changes the projection 
                         * matrix , in then end it updates the Uniform buffer values 
                         * respectively
                         */

                        static void saveConfig();
                        /**
                         * @brief
                         * what this file does is, again take reference to global config
                         * then writes the camera's Position, Front
                         * to the global.toml file
                         */

        };
}

