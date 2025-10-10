#include "global/globalCamera.h"
#include "global/globalConfig.h"
#include "camera/camera.h"
#include "global/globalDisplay.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "toml/toml.hpp"
#include <fstream>
#include <memory>

std::unique_ptr<Camera> global::CameraManager::camera;
float global::CameraManager::m_Near;
float global::CameraManager::m_Far;
glm::mat4 global::CameraManager::projection;
glm::mat4 global::CameraManager::view;

void global::CameraManager::init()
{
        auto config = global::Config::getConfig();

        glm::vec3 cameraPos;
        cameraPos.x = config["camera"]["position"][0].value_or(0.0f);
        cameraPos.y = config["camera"]["position"][1].value_or(0.0f);
        cameraPos.z = config["camera"]["position"][2].value_or(0.0f);

        glm::vec3 cameraFront;
        cameraFront.x = config["camera"]["front"][0].value_or(0.0f);
        cameraFront.y = config["camera"]["front"][1].value_or(0.0f);
        cameraFront.z = config["camera"]["front"][2].value_or(-1.0f);

        camera = std::make_unique<Camera>(cameraPos, cameraFront);        

        m_Near = config["camera"]["near"].value_or(0.1f);
        m_Far = config["camera"]["far"].value_or(100.0f);

        projection = glm::perspective(glm::radians(45.0f), (float)global::Display::getWidth()/global::Display::getheight(), m_Near, m_Far);
        view = camera->GetViewMatrix();

}

void global::CameraManager::update()
{
        view = camera->GetViewMatrix();
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
