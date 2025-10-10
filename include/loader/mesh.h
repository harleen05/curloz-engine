#pragma once

#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "shader/shader.h"


struct Vertex
{
        glm::vec3 v_Position;
        glm::vec3 v_Normal;
        glm::vec2 v_TexCoords;
};

struct Texture 
{
        GLuint t_ID;
        std::string t_type;
        std::string t_path; 
};

class Mesh 
{
        private:
                GLuint m_VAO, m_VBO, m_EBO;
                float m_shininess;
                void init();

        public:
                std::vector<Vertex> m_Vertices;
                std::vector<GLuint> m_Indices;
                std::vector<Texture> m_Textures;

                Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, float shininess);

                void Draw(Shader &shader);
};


