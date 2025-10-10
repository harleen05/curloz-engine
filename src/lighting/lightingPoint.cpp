#include "lighting/lightingPoint.h"
#include "imgui/imgui.h"


LightingPoint::LightingPoint(const std::string &name, glm::vec3 colorComponent[3], glm::vec3 position)
{
        m_name = name;

        m_colorComponent[0] = colorComponent[0];        
        m_colorComponent[1] = colorComponent[1];        
        m_colorComponent[2] = colorComponent[2];        

        m_position = position;
}

glm::vec3 *LightingPoint::getColor() 
{
        return &m_colorComponent[0];
}

void LightingPoint::setColor(glm::vec3 *colorComponent)
{
        m_colorComponent[0] = colorComponent[0];
        m_colorComponent[1] = colorComponent[1];
        m_colorComponent[2] = colorComponent[2];
}

glm::vec3 LightingPoint::getPosition()
{
        return m_position;
}

void LightingPoint::setPosition(glm::vec3 new_position)
{
        m_position = new_position; 
}

void LightingPoint::Edit()
{
        if(ImGui::TreeNode(m_name.c_str()))
        {
                //set position
                glm::vec3 position = this->getPosition();
                ImGui::SliderFloat3("Position", &position.x, -50.0f, 50.0f);
                this->setPosition(position);

                //set component;
                glm::vec3 ambient = m_colorComponent[0];
                ImGui::SliderFloat3("Ambient", &ambient[0], 0.0f, 1.0f);
                glm::vec3 diffuse = m_colorComponent[1];
                ImGui::SliderFloat3("Diffuse", &diffuse[0], 0.0f, 1.0f);
                glm::vec3 specular = m_colorComponent[2];
                ImGui::SliderFloat3("Specular", &specular[0], 0.0f, 1.0f);

                glm::vec3 colorComponents[3] = {ambient, diffuse, specular};
                this->setColor(colorComponents);

                ImGui::TreePop();
        }
}
