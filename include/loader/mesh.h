#pragma once

#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "shader/shader.h"


struct assimp_Vertex
{
        glm::vec3 v_Position;
        glm::vec3 v_Normal;
        glm::vec2 v_TexCoords;
};

struct assimp_Texture 
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
                std::vector<assimp_Vertex> m_Vertices;
                std::vector<GLuint> m_Indices;
                std::vector<assimp_Texture> m_Textures;

                Mesh(std::vector<assimp_Vertex> vertices, std::vector<GLuint> indices, std::vector<assimp_Texture> textures, float shininess);

                void Draw(Shader &shader);
};


