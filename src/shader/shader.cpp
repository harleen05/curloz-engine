#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <error.h>
#include "shader/shader.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(std::string shaderVertexPath, std::string shaderFragmentPath)
{
        if(!std::filesystem::exists(shaderVertexPath))
        {
                std::cerr<<"File: "<<shaderVertexPath<<", does not exits"<<std::endl;
        }
        if(!std::filesystem::exists(shaderFragmentPath))
        {
                std::cerr<<"File: "<<shaderVertexPath<<", does not exits"<<std::endl;
        }

        std::ifstream shaderVertexFile = std::ifstream(shaderVertexPath);
        std::stringstream shaderVertexString;
        shaderVertexString << shaderVertexFile.rdbuf();

        std::ifstream shaderFragmentFile = std::ifstream(shaderFragmentPath);
        std::stringstream shaderFragmentString;
        shaderFragmentString << shaderFragmentFile.rdbuf();
        
        std::string shaderVertexCode = shaderVertexString.str();
        std::string shaderFragmentCode = shaderFragmentString.str();



        GLuint shaderVertex, shaderFragment;
        int success;
        char infoLog[512];

        //vertex shader
        shaderVertex = glCreateShader(GL_VERTEX_SHADER);
        const char *vertex = shaderVertexCode.c_str();
        glShaderSource(shaderVertex, 1, &vertex, NULL);
        glCompileShader(shaderVertex);
        glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
                glGetShaderInfoLog(shaderVertex, 512, NULL, infoLog);
                std::cerr << "Failed to compile shader: "<<shaderVertexPath<<": "<<infoLog<<std::endl;
        }

        //fragment shader
        shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragment = shaderFragmentCode.c_str();
        glShaderSource(shaderFragment, 1, &fragment, NULL);
        glCompileShader(shaderFragment);
        glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
                glGetShaderInfoLog(shaderFragment, 512, NULL, infoLog);
                std::cerr << "Failed to compile shader: "<<shaderFragmentPath<<": "<<infoLog<<std::endl;
        }

        //create program
        m_ID = glCreateProgram();
        glAttachShader(m_ID, shaderVertex);
        glAttachShader(m_ID, shaderFragment);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
                glGetProgramInfoLog(m_ID, 512, NULL, infoLog); 
                std::cerr << "Failed to link program" << infoLog << std::endl;
        }
        
        //delete to save memory
        glDeleteShader(shaderVertex);
        glDeleteShader(shaderFragment);
}

void Shader::use() const
{
        glUseProgram(m_ID);
}


void Shader::setBool(const std::string &name, bool value) const 
{
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const 
{
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const 
{
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const 
{
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()),value.x, value.y, value.z);
}
void Shader::setMat3(const std::string &name, glm::mat3 value) const 
{
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()),1,GL_FALSE,glm::value_ptr(value));
}
void Shader::setMat4(const std::string &name, glm::mat4 value) const 
{
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
