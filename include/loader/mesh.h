#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <string>

#include "shader/shaders.h"

#define MAX_BONE_INFLUENCE 4
//This struct stores information about each triangle in the mesh
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    //4 is the maximum bone influence
    int BoneIDs[MAX_BONE_INFLUENCE];
    float Weights[MAX_BONE_INFLUENCE];
};

//this stores the texture for our triangles
struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

class assimpMesh
{
    private:
        GLuint m_VAO, m_VBO, m_EBO;
        void setupMesh();

    public:
        //Mesh data
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        //setup our mesh
        assimpMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
        //draw our mesh
        void Draw(Shader &shader);


};
