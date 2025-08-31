#pragma once

#include "shader/shaders.h"
#include "loader/mesh.h"
#include "glm/glm.hpp"
#include "texture/imageLoader.h"
#include "assimp_glm_helpers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <map>


struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class Model 
{
    public:
        //constructor
        Model(std::string path);

        //calls all meshes and call their respective draw functions
        void Draw(Shader &shader);	

        std::vector<assimpMesh> meshes;
    private:
        //a vector that holds all our model's meshes
        //directory of texture
        std::string directory;
        //helpful in faster loading
        std::vector<Texture> textures_loaded;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        assimpMesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        GLuint TextureFromFile(const char *path);

        //animation
    public:
        std::map<std::string, BoneInfo> m_BoneInfoMap; 
        int m_BoneCounter = 0;
        void SetVertexBoneDataToDefault(Vertex& vertex);

        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }
        void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
        void extractBoneDataforVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene);


};

