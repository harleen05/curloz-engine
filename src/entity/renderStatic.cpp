#include "entity/renderStatic.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "global/globalCamera.h"
#include "global/globalDisplay.h"
#include "glm/ext/matrix_transform.hpp"
#include "global/globalStatus.h"
#include "imgui/imgui.h"
#include "lighting/lighting.h"
#include <glm/matrix.hpp>
#include <string>

using namespace global;

glm::mat4 renderStatic::projection;
glm::mat4 renderStatic::view;

int renderStatic::m_Count;

void renderStatic::init()
{
        projection = glm::perspective(glm::radians(CameraManager::camera->fov), (float)Display::getWidth() /(float)Display::getheight(), CameraManager::m_Near, CameraManager::m_Far);

        view = CameraManager::camera->GetViewMatrix();

        m_Count = 0;

}



renderStatic::renderStatic (std::string modelPath) : m_Model(modelPath)
{
        model = glm::mat4(1.0f);

}

std::string renderStatic::getName() const 
{
        return m_Name;
}
glm::vec3 renderStatic::getPosition() const 
{
        return m_Position;
}
glm::vec3 renderStatic::getRotation() const 
{
        return m_Rotation;
}
float renderStatic::getScale() const 
{
        return m_Scale;
}
void renderStatic::setName(const std::string &name)
{
        m_Name = name;
}
void renderStatic::setPosition(glm::vec3 Position)
{
        m_Position = Position;

        updateModelMatrix();
}
void renderStatic::setRotation(glm::vec3 Rotation)
{
        m_Rotation = Rotation;
        updateModelMatrix();
}
void renderStatic::setScale(float scale)
{
        m_Scale = scale;
        updateModelMatrix();
}
Shader &renderStatic::getShader()
{
        return m_Shader;
}
void renderStatic::updateModelMatrix()
{
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_Position);
        model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_Scale));
}


void renderStatic::update(float deltaTime)
{
        view = CameraManager::camera->GetViewMatrix();

        m_Shader.use();

        if (global::Status::Mode == global::MODE::EDIT_MODE)
        {
                if (ImGui::TreeNode(m_Name.c_str()))
                {
                        //Set Position
                        glm::vec3 entityPos = this->getPosition();
                        ImGui::SliderFloat3("Position", &entityPos.x, -50.0f, 50.0f);
                        this->setPosition(entityPos);

                        //set Rotation
                        glm::vec3 entityRot = this->getRotation();
                        ImGui::SliderFloat3("Rotation", &entityRot.x, -180.0f, 180.0f);
                        this->setRotation(entityRot);

                        //set Scale
                        float entityScale = this->getScale();
                        ImGui::SliderFloat("Scale", &entityScale, 0.1f, 10.0f);
                        this->setScale(entityScale);


                        ImGui::TreePop();
                }
        }

        m_Shader.setMat4("projection", renderStatic::projection);
        m_Shader.setMat4("view", renderStatic::view);
        m_Shader.setMat4("model", this->model);

        m_Shader.setVec3("directionalLight.direction", Lighting::getGlobalLight_Direction());
        m_Shader.setVec3("directionalLight.ambient", Lighting::getGlobalLight_Color()[0]);
        m_Shader.setVec3("directionalLight.diffuse", Lighting::getGlobalLight_Color()[1]);
        m_Shader.setVec3("directionalLight.specular", Lighting::getGlobalLight_Color()[2]);

        m_Shader.setInt("numPointLights", Lighting::getNumPointLights());
        for(int i=0; i<Lighting::getNumPointLights(); ++i)
        {
                        m_Shader.setVec3("pointLight[" + std::to_string(i) + "].position", Lighting::m_point_lights[i]->getPosition());

                        m_Shader.setVec3("pointLight[" + std::to_string(i) + "].ambient", Lighting::m_point_lights[i]->getColor()[0]);
                        m_Shader.setVec3("pointLight[" + std::to_string(i) + "].diffuse", Lighting::m_point_lights[i]->getColor()[1]);
                        m_Shader.setVec3("pointLight[" + std::to_string(i) + "].specular", Lighting::m_point_lights[i]->getColor()[2]);
        }

        m_Shader.setMat3("normalCalculationMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
        m_Shader.setVec3("cameraPosition", global::CameraManager::camera->getPosition());

        m_Model.Draw(m_Shader);

}
