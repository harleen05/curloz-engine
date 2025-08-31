#include "loader/mesh.h"
#include <cstddef>
#include <string>

assimpMesh::assimpMesh(std::vector<Vertex> vertices, 
        std::vector<GLuint> indices, 
        std::vector<Texture> textures)
{
    //setup the attributes
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    //set our mesh
    setupMesh();
}

void assimpMesh::setupMesh()
{
    //generate our buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                    &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
            &indices[0], GL_STATIC_DRAW);

    //set our shader locations
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


void assimpMesh::Draw(Shader &shader)
{
    GLuint numDiffuse = 0;
    GLuint numSpecular = 0;

    for(GLuint i=0; i<textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i); 

        if(textures[i].type == "texture_diffuse")
        {
            shader.setInt(("material." + textures[i].type + std::to_string(numDiffuse) ).c_str(), i);
            ++numDiffuse;
        } 
        else if(textures[i].type == "texture_specular")
        {
            shader.setInt(("material." + textures[i].type + std::to_string(numSpecular) ).c_str(), i);
            ++numSpecular;
        } 

        glBindTexture(GL_TEXTURE_2D, textures[i].id);

    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}
