/**
 * @file mesh.h
 * @author Aditya Saini
 * @brief loads mesh
 *
 */

#pragma once

#include <vector>
#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "shader/shader.h"


/**
 * @brief
 * holds the data of vertex
 *
 */
struct assimp_Vertex
{
        glm::vec3 v_Position;
        glm::vec3 v_Normal;
        glm::vec2 v_TexCoords;
};

/**
 * @brief
 * holds data about texture
 *
 */
struct assimp_Texture 
{
        GLuint t_ID;
        std::string t_type;
        std::string t_path; 
};

/**
 * @brief
 * Mesh class holds information about meshes in any model
 *
 * @detail
 * This is the mesh class, mesh are like sub parts in any model
 * In order to make any object of mesh class, pass array/vector containing 
 * vertices, indices, texture and the shininess of the mesh.
 *
 */
class Mesh 
{
        private:
                /// Holds the GPU data
                GLuint m_VAO, m_VBO, m_EBO;
                
                ///Holds the shininess of mesh
                float m_shininess;

                void init();
                /**
                 * @brief
                 * prepares the gpu by passing relevant VAO's VBO's and EBO's
                 *
                 * @detail
                 * Create vertex array object of current object's m_VAO, the relevant VBO and EBO
                 * enables information about vertices in shader location 0, normal in location 1 and texture coordinates in location 2
                 * the offset is passed as is like present in the Vertex struct -> first Positon, then normal, then texture coordinates
                 *
                 * @note
                 * dont forget to deallocate the m_VAO at the end
                 *
                 */

        public:
                /// The vector storing vertice data of all the vertices in mesh
                std::vector<assimp_Vertex> m_Vertices;

                /// The vector storing indice data of all the vertices
                std::vector<GLuint> m_Indices;

                /// The vector storing all textures for relevant vertices of mesh
                std::vector<assimp_Texture> m_Textures;

                Mesh(std::vector<assimp_Vertex> vertices, std::vector<GLuint> indices, std::vector<assimp_Texture> textures, float shininess);
                /**
                 * @brief
                 * Constructor of Mesh class
                 *
                 * @detail
                 * The mesh constructor expects 4 parameters,
                 * 1. std::vector of assimp_vertex(vertex data)
                 * 2. std::vector of unsigned int(indices data)
                 * 3. std::vector of assimp_Texture(texture data)
                 * 4. (temporary) float of shininess of mesh
                 *
                 * Then initializes the in-object data with the arguments passed
                 * and calls the initialization function (init())
                 *
                 */

                void Draw(Shader &shader);
                /**
                 * @brief
                 * Draws the mesh
                 *
                 * @detail
                 * It accepts a shader reference, then first of all initializes it
                 * Initializes these three variables ->
                 * 1. numDiffuseMaps
                 * 2. numSpecularMaps
                 * 3. numSpecularMaps
                 * to 0
                 *
                 * then loops over all textures present in the current object's m_Texture vector
                 * and uploads their data to GPU/shader
                 * The procedure for it is to set them depending on the texture type, 
                 * Say texture type is "texture_diffuse" then pass the data to shader as ->
                 * "material.texture_diffuse" + "numDiffuseMaps" 
                 * do the same for specular and emission maps
                 * also, if emission and specular maps are zero, make sure to pass material.hasEmission/hasSpecular to 0 , and 1 otherwise
                 *
                 * in the end, make sure to draw the GL_TRIANGLES using indices, not arrays
                 *
                 */
};
