#pragma once

#include <memory>
#include "glm/vec3.hpp"
#include "loader/model.h"
#include "camera/camera.h"
#include "global/globalEntity.h"
#include "shader/shader.h"


class renderStatic 
{
        public:
                static void init();
                static int m_Count;
                static void saveConfig();

        private:
                static glm::mat4 projection;
                static glm::mat4 view;

                Model m_Model;
                std::string m_Name;

                glm::vec3 m_Position = glm::vec3(0.0f);
                glm::vec3 m_Rotation = glm::vec3(0.0f);
                float m_Scale = 1.0f;

                glm::mat4 model = glm::mat4(1.0f);
                void updateModelMatrix();

                Shader m_Shader = Shader(global::Entity::getVertexShader(), global::Entity::getFragmentShader());

        public:
                renderStatic (std::string modelPath);
                void update (float deltaTime);

                std::string getName() const;
                glm::vec3 getPosition() const;
                glm::vec3 getRotation() const;
                float getScale() const;
                Shader &getShader();

                void setName(const std::string &name);
                void setPosition(glm::vec3 Position);
                void setRotation(glm::vec3 Rotation);
                void setScale(float Scale);

};
