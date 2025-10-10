#pragma once

#include "glm/vec3.hpp"
#include "shader/shader.h"


class lightingDirectional
{
        private:
                glm::vec3 colorComponent[3]; //Stores 0->ambient, 1->diffuse, 2->specular

                glm::vec3 direction;

        public:
                glm::vec3 *getColor();
                void setColor(glm::vec3 colorComponents[3]);
                glm::vec3 getDirection();
                void setDirection(glm::vec3 new_direction);

                lightingDirectional(){}
                lightingDirectional (glm::vec3 colorComponents[3], glm::vec3 Direction);

                void Edit();
};
