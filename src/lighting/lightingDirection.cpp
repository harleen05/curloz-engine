#include "lighting/lightingDirection.h"
#include "imgui/imgui.h"

lightingDirectional::lightingDirectional (glm::vec3 *colorComponents, glm::vec3 Direction) 
{
        direction = Direction;

        colorComponent[0] = colorComponents[0];
        colorComponent[1] = colorComponents[1];
        colorComponent[2] = colorComponents[2];

}

glm::vec3 *lightingDirectional::getColor ()
{
       return colorComponent;
}

void lightingDirectional::setColor (glm::vec3 *colorComponents)
{
        colorComponent[0] = colorComponents[0];
        colorComponent[1] = colorComponents[1];
        colorComponent[2] = colorComponents[2];
}

void lightingDirectional::setDirection(glm::vec3 new_direction)
{
        direction = new_direction;
}

glm::vec3 lightingDirectional::getDirection()
{
        return direction;
}

void lightingDirectional::Edit()
{
        glm::vec3 ambient = colorComponent[0];
        ImGui::SliderFloat3("Ambient", &ambient[0], 0.0f, 1.0f);
        glm::vec3 diffuse = colorComponent[1];
        ImGui::SliderFloat3("Diffuse", &diffuse[0], 0.0f, 1.0f);
        glm::vec3 specular = colorComponent[2];
        ImGui::SliderFloat3("Specular", &specular[0], 0.0f, 1.0f);

        glm::vec3 colorComponents[3] = {ambient, diffuse, specular};
        setColor(colorComponents);


        glm::vec3 temp_direction = getDirection();
        ImGui::SliderFloat3("Direction", &temp_direction[0], -1.0f, 1.0f);
        setDirection(temp_direction);

}


