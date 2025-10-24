#include <iostream>
#include "loader/model.h"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "global/globalStatus.h"
#include "texture/imageloader.h"


Model::Model (const std::string &modelPath)
{
        if(!loadModel(modelPath))
                std::cerr << "Could not load model: " << modelPath << std::endl;


        if (global::Status::Profile == global::PROFILE::DEBUG_MODE)
                std::cout << "loaded model: " << modelPath << std::endl;
}

bool Model::loadModel (const std::string &modelPath)
{
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(modelPath,
                        aiProcess_Triangulate |
                        aiProcess_FlipUVs
        );
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
                std::cerr << importer.GetErrorString() << std::endl;
                return false ;
        }

        directory = modelPath.substr(0, modelPath.find_last_of('/'));


        processNode (scene->mRootNode, scene);

        return true;
}

void Model::processNode (aiNode *node, const aiScene *scene)
{
        for (GLuint i=0; i<node->mNumMeshes; ++i)
        {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                m_Meshes.push_back(processMesh(mesh, scene));
        }

        for(GLuint i=0; i<node->mNumChildren; ++i)
        {
                processNode(node->mChildren[i], scene);
        }
}

Mesh Model::processMesh (aiMesh *mesh, const aiScene *scene)
{

        std::vector<assimp_Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<assimp_Texture> textures;

        vertices.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                assimp_Vertex vertex; 

                vertex.v_Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

                if(mesh->HasNormals()) 
                {
                        vertex.v_Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                } 
                else 
                {
                        vertex.v_Normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default normal
                }

                if(mesh->mTextureCoords[0]) 
                {
                        vertex.v_TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                } 
                else 
                {
                        vertex.v_TexCoords = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
        }

        indices.reserve(mesh->mNumFaces * 3);
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) 
        {
                aiFace face = mesh->mFaces[i];
                for(GLuint j = 0; j < face.mNumIndices; j++) 
                {
                        indices.push_back(face.mIndices[j]);
                }
        }

        float shininess = 128.0f;
        if (mesh->mMaterialIndex >= 0) 
        {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                std::vector<assimp_Texture> diffuseMaps = loadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

                std::vector<assimp_Texture> specularMaps = loadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

                std::vector<assimp_Texture> emissionMaps = loadMaterial(material, aiTextureType_EMISSIVE, "texture_emission");
                textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());

                aiReturn result = material->Get(AI_MATKEY_SHININESS, shininess);
                if(result != AI_SUCCESS || shininess < 1.0f) 
                {
                        shininess = 128.0f;
                }


        }

        return Mesh (vertices, indices, textures, shininess);
}

std::vector<assimp_Texture> Model::loadMaterial(aiMaterial *mat, aiTextureType type, std::string typeName) {
        std::vector<assimp_Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip = false;
                for(unsigned int j = 0; j < m_texturesLoaded.size(); j++) 
                {
                        if(std::strcmp(m_texturesLoaded[j].t_path.data(), str.C_Str()) == 0) 
                        {
                                textures.push_back(m_texturesLoaded[j]);
                                skip = true;
                                break;
                        }
                }

                if(!skip) 
                {
                        assimp_Texture texture;

                        texture.t_ID = loadTexture(str.C_Str() );
                        texture.t_type = typeName;
                        texture.t_path = str.C_Str();
                        textures.push_back(texture);
                        m_texturesLoaded.push_back(texture);
                }
        }
        return textures;
}

GLuint Model::loadTexture(const std::string &texturePath)
{
        std::string filename = std::string(texturePath);
        if(filename[0] == '/') 
        {
                filename = filename.substr(1);
        }
        filename = directory + '/' + filename;
        std::replace(filename.begin(), filename.end(),'\\','/');


        bool flip = false;
        GLuint textureID = Texture::loadImageFromFile(filename.c_str(), flip);
        return textureID;

}

void Model::Draw(Shader &shader)
{
        shader.use();
        for(GLuint i=0; i<m_Meshes.size(); ++i)
        {
                m_Meshes[i].Draw(shader);
        }

}
