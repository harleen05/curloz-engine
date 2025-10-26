/**
 * @file model.h
 * @author Aditya Saini
 * 
 * @brief 
 * loads our model :)
 *
 */

#pragma once

#include <map>
#include <string>
#include "loader/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "shader/shader.h"


/**
 * @brief
 * This class loads models passed into our game
 * currently use assimp for loading models
 *
 */
class Model
{
        public:
                Model(const std::string &modelPath);
                /**
                 * @brief
                 * Model constructor, 
                 * takes in the path to model as parameter
                 *
                 * @detail
                 * Constructor expects the path to our model
                 * Then loads the model using loadModel function whcih returns a bool whether model loaded properly or not
                 * if not, and engine will print that model didnt load
                 * if it did, and global::Profile is set to debug, then it will just inform model loaded properly
                 *
                 */

                void Draw(Shader &shader);
                /**
                 * @brief
                 * This function draws our meshes
                 *
                 * @detail
                 * Loops over all the meshes objects in m_Meshes vector
                 * and calls their respective draw function by passing the incoming shader argument to it
                 *
                 */

        private:
                /// Holds all our meshes of the model
                std::vector<Mesh> m_Meshes;

                /// The parent directory of our model, useful in determining absolute path of the model's textures
                std::string directory;

                /// vector holding data of all the loaded textures, useful in faster loading cuz many vertices/meshs use same texture, so rather then reloading new again, just pass older one
                std::vector<assimp_Texture> m_texturesLoaded;

                bool loadModel (const std::string &modelPath);
                /**
                 * @brief 
                 * The main function loading our model, 
                 * takes the path to model
                 *
                 * @note
                 * Please pass model path relative to @root directory
                 *
                 * @detail
                 * This function takes path of model passed by the constructor
                 * then create an Assimp::Importer object which loads our model 
                 * using these flags -> 
                 * 1. aiProcess_Triangulate 
                 * 2. aiProcess_FlipUVs
                 *
                 * the loaded model is stored in 'scene' , a const pointer of aiScene class
                 * after that, we check whether model loaded properly, or returned any warnings or errors
                 * and prints if it didnt
                 *
                 * Sets directory to parent directory of model
                 *
                 * Then calls the processNode function which processes all the nodes of our model
                 *
                 */

                void processNode (aiNode *node, const aiScene *scene);
                /**
                 * @brief
                 * processNode function loads all the nodes present in the model
                 *
                 * @detail
                 * This function takes a aiNode pointer and const aiScene pointer as parameter
                 * This then loops over all meshes present in the node 
                 * and creates a new aiMesh pointer which holds pointer to the current mesh
                 * this mesh is then pushed over to be processes over processMesh function 
                 * and this returned mesh is pushed back to the m_Meshes vector
                 *
                 * After this, we recursively follow the same above procedure for all the childrens of this node
                 *
                 */

                Mesh processMesh (aiMesh *mesh, const aiScene *scene);
                /**
                 * @warning
                 * Very huge function
                 *
                 * @brief
                 * This function loads our meshes for our model, 
                 * and return a mesh object
                 *
                 * @detail
                 *
                 * This function expects 2 arguments ->
                 * 1. aiMesh pointer
                 * 2. const aiScene pointer
                 *
                 * Then creates 3 std::vectors ->
                 * 1. vertices, of assimp_Vertex (present in mesh.h)
                 * 2. indices, of unsigned int
                 * 3. texures, of assimp_Texture (present in mesh.h)
                 *
                 * After this, it loops over number of vertices present in passed mesh
                 * inside loop, it creates a new assimp_Vertex(present in mesh.h) object, call it vertex for ex.
                 * then, we initialize vertex's data with current looped vertice's data
                 * then we pass vertex to vertices vector created above
                 *
                 * Then we do so the same for indices
                 *
                 * Then over to textures, we first check if they are present :/
                 * We create a aiMaterial pointer, which holds material index, indexed by scene's material index (ok wtf!!?? js skip this)
                 * after that, we create vectors of different types of textures (diffuse, emission, specular)
                 * which holds vector return by the loadMaterial function
                 * At last we check if material has shininess ,and set shininess to it
                 *
                 * then return mesh object with vertices indices textures and shininess as arguments
                 *
                 */

                std::vector<assimp_Texture> loadMaterial (aiMaterial *texture, aiTextureType textureType, std::string typeName);
                /**
                 * @brief
                 * Necessary to load our materials for our model
                 *
                 * @detail
                 * Takes pointer of assimp's aiMaterial, aiTextureType, and type name as argeuments
                 * Makes a new vector of assimp_Texture named textures
                 * Then we loop over number of texture count returned by assimp for current material
                 *
                 * First we check if the current texture is already present in our m_texturesLoaded vector, 
                 * by using path as referencing factor
                 * if yes, then we just push back the aready loaded texture to textures
                 * if not, we then load a new texture onto the textures vector using loadTexture function
                 *
                 * @note
                 * if textyre type is "texture_diffuse" or "texture_emission" then we gamma correct it,
                 * and pass true while loading the texture
                 *
                 */

                GLuint loadTexture (const std::string &texturePath, bool gammaCorrect);
                /**
                 * @brief 
                 * loads aur images/textures of our model
                 *
                 * @detail
                 * accepts path to texture and a bool to check whether we need to gamma correct it or not
                 * then we load the texture using our own loadImageFromFile function (texture/imageLoader.h) present in Texture class
                 * the function returns an opengl unsigned int 
                 * we return this returned unsigned int
                 *
                 * @note
                 * in order to imporve cross platform compatibility, we replace all '\' with '/'
                 * :w
                 * 
                 */

};
