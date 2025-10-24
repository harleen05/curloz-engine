#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera/camera.h"
#include "glm/trigonometric.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"


Camera::Camera (glm::vec3 position, glm::vec3 front) :
        Position(position), 
        Front(front),
        MovementSpeed(SPEED), 
        MouseSensitivity(SENSITIVITY), 
        Zoom(ZOOM)
{
        Yaw = YAW;
        Pitch = PITCH;
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
}





glm::mat4 Camera::GetViewMatrix()
{
        return glm::lookAt(Position,Position + Front , Up);

}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime)
{

        float velocity = (shouldMove) ? MovementSpeed * deltaTime : 0.0f;

        if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) 
                Position += Front * velocity;
        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) 
                Position -= Front * velocity;
        if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) 
                Position -= Right * velocity;
        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) 
                Position += Right * velocity;
        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
                Position -= WorldUp * velocity;
        if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS) 
                Position += WorldUp * velocity;

}

void Camera::ProcessMouseMovement(double xpos, double ypos)
{
        float xPos = static_cast<float>(xpos);
        float yPos = static_cast<float>(ypos);

        if(firstInput)
        {
                lastX = xPos;
                lastY = yPos;
                firstInput = false;
        }

        float xoffset = xPos - lastX;
        float yoffset = lastY - yPos;

        lastX = xPos;
        lastY = yPos;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if(Pitch >= 89.0f) 
                Pitch = 89.0f;

        if(Pitch <= -89.0f) 
                Pitch = -89.0f;

        if (!shouldMove)
                return;

        updateCameraVectors();

}

void Camera::ProcessMouseScroll(float yoffset)
{
        Zoom -= yoffset;
        if(Zoom <= 1.0f) Zoom = 1;
        if(Zoom >= 89.0f) Zoom = 89.0f;

}

void Camera::updateCameraVectors()
{
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front,WorldUp));
        Up = glm::normalize(glm::cross(Right,Front));
    
}

void Camera::processScrollBack(double yOffset)
{
        fov -= yOffset;
        if(fov <= 1) fov = 1;
        if(fov >= 89) fov = 89;

        check_FOV_changed = true;
}

glm::vec3 Camera::getPosition() const 
{
        return Position;
}
glm::vec3 Camera::getFront() const 
{
        return Front;
}
