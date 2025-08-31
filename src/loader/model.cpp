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
        SetVertexBoneDataToDefault(vertex);
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
    //
    std::cout<<"Processing its normals"<<std::endl;
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
        std::cout << "Checking all texture types:\n";
        for (int i = 1; i < AI_TEXTURE_TYPE_MAX; ++i)
        {
            aiTextureType type = static_cast<aiTextureType>(i);
            unsigned int count = material->GetTextureCount(type);
            if (count > 0)
            {
                std::cout << "Type " << i << " (" << type << ") has " << count << " textures\n";
                for (unsigned int t = 0; t < count; ++t)
                {
                    aiString str;
                    material->GetTexture(type, t, &str);
                    std::cout << "  Texture " << t << ": " << str.C_Str() << std::endl;
                }
            }
            else std::cout<<"Texture count is zero"<<std::endl;
        }
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        if(!diffuseMaps.empty()) std::cout<<"Found models Diffuse maps"<<std::endl;
        else std::cout<<"didnt find any diffuse maps for model"<<std::endl;

        std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        if(!specularMaps.empty()) std::cout<<"Found models specular maps"<<std::endl;
        else std::cout<<"didnt find any specular maps for model"<<std::endl;
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    std::cout << "Mesh processed successfully!" << std::endl;


    extractBoneDataforVertices(vertices, mesh, scene);
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
    std::cout<<"Loading obj/fbx texture-> "<<filename<<std::endl;

    GLuint textureID = loadTextureFromFile(filename.c_str(),false);
    return textureID;
}



void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.BoneIDs[i] = -1;
        vertex.Weights[i] = 0.0f;
    }
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
        // Skip negligible weights
    if (weight < 0.001f) return;

    // Check if this bone is already assigned to this vertex
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (vertex.BoneIDs[i] == boneID) {
            vertex.Weights[i] += weight; // Add to existing weight
            return;
        }
    }

    // Find empty slot
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (vertex.BoneIDs[i] < 0) {
            vertex.BoneIDs[i] = boneID;
            vertex.Weights[i] = weight;
            return;
        }
    }

    // All slots full - replace weakest influence
    int weakestIndex = 0;
    for (int i = 1; i < MAX_BONE_INFLUENCE; ++i) {
        if (vertex.Weights[i] < vertex.Weights[weakestIndex]) {
            weakestIndex = i;
        }
    }

    if (weight > vertex.Weights[weakestIndex]) {
        vertex.BoneIDs[weakestIndex] = boneID;
        vertex.Weights[weakestIndex] = weight;
    }
}

void Model::extractBoneDataforVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{

    std::cout << "=== MESH BONE DEBUG ===" << std::endl;
    std::cout << "Mesh has " << mesh->mNumBones << " bones" << std::endl;
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        // std::cout << "Processing bone: " << boneName << std::endl;
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(
            mesh->mBones[boneIndex]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = m_BoneCounter;
            m_BoneCounter++;
            // std::cout << "  NEW bone - assigned ID: " << boneID << std::endl;
        }
        else
        {
            boneID = m_BoneInfoMap[boneName].id;
            std::cout << "  EXISTING bone - found ID: " << boneID << std::endl;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;
        std::cout<<"numWeights: "<<numWeights<<std::endl;
        // std::cout << "  Affects " << numWeights << " vertices:" << std::endl;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            // std::cout << "    Vertex " << vertexId << " weight: " << weight << std::endl;
            assert(vertexId < vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    std::cout << "Final model bone count: " << m_BoneCounter << std::endl;
    }
    // Add this at the end of extractBoneDataforVertices
    std::cout << "Normalizing weights..." << std::endl;
    for (auto& vertex : vertices) {
        float totalWeight = 0.0f;
        
        // Calculate total
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            if (vertex.BoneIDs[i] >= 0) {
                totalWeight += vertex.Weights[i];
            }
        }
        
        // Normalize to sum to 1.0
        if (totalWeight > 0.001f) {
            for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
                if (vertex.BoneIDs[i] >= 0) {
                    vertex.Weights[i] /= totalWeight;
                }
            }
        } else {
            // Fallback: assign completely to bone 0
            vertex.BoneIDs[0] = 0;
            vertex.Weights[0] = 1.0f;
            for (int i = 1; i < MAX_BONE_INFLUENCE; i++) {
                vertex.BoneIDs[i] = -1;
                vertex.Weights[i] = 0.0f;
            }
        }
    }
}
