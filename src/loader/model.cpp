#include "assimp/postprocess.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "loader/model.h"
#include <algorithm>

Model::Model(std::string path)
{
    loadModel(path);
}
void Model::Draw(Shader &shader)
{
    // std::cout<<"calling draw function"<<std::endl;
    // std::cout<<"For numMeshes->"<<meshes.size()<<std::endl;
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}  

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | 
                                                 aiProcess_FlipUVs | 
                                                 aiProcess_ConvertToLeftHanded | 
                                                 aiProcess_LimitBoneWeights|
                                                 aiProcess_ValidateDataStructure |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_ImproveCacheLocality |
                                                 aiProcess_GenSmoothNormals 
                                            );	

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP-> " << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    std::cout<<"loaded model: "<<path<<std::endl;

    processNode(scene->mRootNode, scene);
}  

void Model::processNode(aiNode *node, const aiScene *scene)
{

    // std::cout << "Processing node: " << node->mName.C_Str()<< " with " << node->mNumMeshes << " meshes" << std::endl;
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

assimpMesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // std::cout << "Processing mesh with " << mesh->mNumVertices << " vertices" << std::endl;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;


    // std::cout<<"Processing its vertices"<<std::endl;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    
    // process material
    std::cout<<"Processing its textures"<<std::endl;
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        for (int i = 1; i < AI_TEXTURE_TYPE_MAX; ++i)
        {
            aiTextureType type = static_cast<aiTextureType>(i);
            unsigned int count = material->GetTextureCount(type);
            if (count > 0)
            {
                for (unsigned int t = 0; t < count; ++t)
                {
                    aiString str;
                    material->GetTexture(type, t, &str);
                }
            }
        }
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // if(!diffuseMaps.empty()) std::cout<<"Found models Diffuse maps"<<std::endl;

        std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        // if(!specularMaps.empty()) std::cout<<"Found models specular maps"<<std::endl;
        // else std::cout<<"didnt find any specular maps for model"<<std::endl;
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }


    return assimpMesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {   
            Texture texture;
            texture.id = TextureFromFile(str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); 
        }
    }
    return textures;
}  

GLuint Model::TextureFromFile(const char *path)
{
    std::string filename = std::string(path);
    if(filename[0] == '/')
    {
         filename = filename.substr(1);
    }
    filename = directory + '/' + filename;
    std::replace(filename.begin(), filename.end(),'\\','/');

    GLuint textureID = loadTextureFromFile(filename.c_str(),false);
    return textureID;
}




