#pragma once 

#include "shader/shader.h"

class screenFramebuffer
{
	public:
		static GLuint quadVAO;
		static Shader shader;

		static void Init();
		static void Draw(GLuint screenTexture);

};

class renderFramebuffer
{
	public:
		static GLuint FBO;
		static GLuint textureColorBuffer, RBO;

		static bool Init(int screenWidth, int screenHeight);
		
		inline static GLuint getFramebuffer()
		{
			return FBO;
		}
		inline static GLuint getScreenTexture()
		{
			return textureColorBuffer;
		}

};
