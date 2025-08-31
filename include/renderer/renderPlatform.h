#pragma once 

#include "glad/glad.h"
#include "global/general.h"
#include "shader/shaders.h"
#include "texture/imageLoader.h"

class renderPlatform
{
    private:
        static GLuint VAO;
        static Shader platformShader;
        static GLuint platformDiffuse;
        static GLuint platformSpecular;
    public:
        static void init();
        static void Draw();
};

