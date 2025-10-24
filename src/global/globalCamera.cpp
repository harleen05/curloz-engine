#include "glad/glad.h"
#include <fstream>
#include <memory>
#include "global/globalCamera.h"
#include "global/globalConfig.h"
#include "camera/camera.h"
#include "global/globalDisplay.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "toml/toml.hpp"

std::unique_ptr<Camera> global::CameraManager::camera;
float global::CameraManager::m_Near;
float global::CameraManager::m_Far;
GLuint global::CameraManager::UBO_MATRICES;
glm::mat4 global::CameraManager::projection;
glm::mat4 global::CameraManager::view;

void global::CameraManager::init()
{
        auto config = global::Config::getConfig();

	      float fov = config["camera"]["fov"].value_or(75.0f);

        glm::vec3 cameraPos;
        cameraPos.x = config["camera"]["position"][0].value_or(0.0f);
        cameraPos.y = config["camera"]["position"][1].value_or(0.0f);
        cameraPos.z = config["camera"]["position"][2].value_or(0.0f);

        glm::vec3 cameraFront;
        cameraFront.x = config["camera"]["front"][0].value_or(0.0f);
        cameraFront.y = config["camera"]["front"][1].value_or(0.0f);
        cameraFront.z = config["camera"]["front"][2].value_or(-1.0f);

        m_Near = config["camera"]["near"].value_or(0.1f);
        m_Far = config["camera"]["far"].value_or(100.0f);



        camera = std::make_unique<Camera>(cameraPos, cameraFront);        

        projection = glm::perspective(glm::radians(fov), (float)global::Display::getWidth()/global::Display::getheight(), m_Near, m_Far);
        view = camera->GetViewMatrix();

        glGenBuffers(1, &UBO_MATRICES);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO_MATRICES);
        glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_MATRICES, 0, 2*sizeof(glm::mat4));

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
}

void global::CameraManager::update()
{
        glBindBuffer(GL_UNIFORM_BUFFER, UBO_MATRICES);

        view = camera->GetViewMatrix();
        if (camera->check_FOV_changed)
        {
                projection = glm::perspective(glm::radians(camera->fov), (float)global::Display::getWidth()/global::Display::getheight(), m_Near, m_Far);
                camera->check_FOV_changed = false;
        }

        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));


        glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void global::CameraManager::saveConfig()
{
        auto config = global::Config::getConfig();
        
        toml::table *cameraTable = config["camera"].as_table();

        toml::array *positionArray = (*cameraTable)["position"].as_array();
        glm::vec3 position = global::CameraManager::camera->getPosition();
        *positionArray = toml::array(position.x, position.y, position.z);

        toml::array *frontArray = (*cameraTable)["front"].as_array();
        glm::vec3 front = global::CameraManager::camera->getFront();
        *frontArray = toml::array(front.x,front.y, front.z);


        std::ofstream file("assets/config/global.toml");
        file << config; 
        file.close();
}
