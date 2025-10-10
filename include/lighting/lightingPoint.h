#pragma once

#include <exception>
#include <string>
#include "glm/vec3.hpp"

class LightingPoint 
{
        private:
                std::string m_name;
                glm::vec3 m_position = glm::vec3(0.0f);
                glm::vec3 m_colorComponent[3] = {glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)}; //Stores 0->ambient, 1->diffuse, 2->specular

        public:
                glm::vec3 *getColor();
                void setColor(glm::vec3 *colorComponent);

                glm::vec3 getPosition();
                void setPosition(glm::vec3 position);

                LightingPoint();
                LightingPoint(const std::string &name, glm::vec3 colorComponent[3], glm::vec3 position);

                void Edit();

};
