#pragma once

#include <map>
#include <string>
#include "loader/mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "shader/shader.h"


class Model
{
        public:
                Model(const std::string &modelPath);
                void Draw(Shader &shader);

                std::vector<Mesh> m_Meshes;

        private:
                std::string directory;
                std::vector<assimp_Texture> m_texturesLoaded;

                bool loadModel (const std::string &modelPath);
                void processNode (aiNode *node, const aiScene *scene);
                Mesh processMesh (aiMesh *mesh, const aiScene *scene);
                std::vector<assimp_Texture> loadMaterial (aiMaterial *texture, aiTextureType textureType, std::string typeName);
                GLuint loadTexture (const std::string &texturePath);

};
