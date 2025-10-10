#pragma once

#include <memory>
#include "camera/camera.h"

namespace global 
{
        class CameraManager
        {
                public:
                        static std::unique_ptr<Camera> camera;

                        static float m_Near;
                        static float m_Far;

                        static glm::mat4 projection;
                        static glm::mat4 view;

                        static void init();
                        static void update();

                        static void saveConfig();
        };
}

