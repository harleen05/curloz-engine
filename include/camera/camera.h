#pragma once

#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

enum Camera_Movement
{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        STOP
};

const float YAW = 0.0f;
const float PITCH = -10.0f;
const float SPEED = 4.0f;
const float SENSITIVITY = 0.3f;
const float ZOOM = 75.0f;

class Camera
{
        private:
                glm::vec3 Position;
                glm::vec3 Front;
                glm::vec3 Up;
                glm::vec3 Right;
                glm::vec3 WorldUp;

                float Yaw;
                float Pitch;

                float MovementSpeed;
                float MouseSensitivity;
                float Zoom;
                float aspectRatio;
                float lastX;
                float lastY;


        public:
                glm::vec3 getPosition() const;
                glm::vec3 getFront() const;

                bool check_FOV_changed = false;
                bool firstInput = true;
                bool shouldMove = false;
                float fov = ZOOM;
                Camera_Movement Direction;


                Camera (
                                glm::vec3 position,
                                glm::vec3 front
                       );

                glm::mat4 GetViewMatrix();

                void ProcessKeyboard(GLFWwindow* window, float deltaTime);
                void ProcessMouseMovement(double xpos, double ypos);
                void ProcessMouseScroll(float yoffset);
                void processScrollBack(double yOffset);

                void updateCameraVectors();

};



