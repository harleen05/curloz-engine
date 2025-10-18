#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

class Shader 
{
        public:

        GLuint m_ID;
        Shader(){}
        Shader(std::string shaderVertexPath, std::string shaderFragmentPath);

        //use this shader
        void use() const;

        //Set shader parameters
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
        void setMat3(const std::string &name, glm::mat3 value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;
};
