#include "loader/mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, float shininess)
{
        m_Vertices = vertices;
        m_Indices = indices;
        m_Textures = textures;
        m_shininess = shininess;

        init();
}

void Mesh::init()
{
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);


        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, v_Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, v_TexCoords));


        glBindVertexArray(0);

}
        
void Mesh::Draw(Shader &shader)
{
        shader.use();

        GLuint numDiffuseMaps = 0;
        GLuint numSpecularMaps = 0;
        GLuint numEmissionMaps = 0;

        for (GLuint i=0; i<m_Textures.size(); ++i)
        {
                glActiveTexture(GL_TEXTURE0 + i);


                if (m_Textures[i].t_type == "texture_diffuse") 
                {
                        shader.setInt(("material." + m_Textures[i].t_type + std::to_string(numDiffuseMaps) ).c_str(), i);
                        ++numDiffuseMaps;
                }
                else if (m_Textures[i].t_type == "texture_specular")
                {
                        shader.setInt(("material." + m_Textures[i].t_type + std::to_string(numSpecularMaps) ).c_str(), i);
                        ++numSpecularMaps;
                }
                else if (m_Textures[i].t_type == "texture_emission")
                {
                        shader.setInt(("material." + m_Textures[i].t_type + std::to_string(numEmissionMaps) ).c_str(), i);
                        ++numEmissionMaps;
                }



                
                glBindTexture(GL_TEXTURE_2D, m_Textures[i].t_ID);
        }

        
        glActiveTexture(GL_TEXTURE0);

        shader.setFloat("material.shininess", m_shininess);
        if(numSpecularMaps == 0)
        {
                shader.setBool("material.hasSpecular", 0);
        }
        else 
        {
                shader.setBool("material.hasSpecular", 1);
        }
        if(numEmissionMaps== 0)
        {
                shader.setBool("material.hasEmission", 0);
        }
        else 
        {
                shader.setBool("material.hasEmission", 1);
        }


        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

}
