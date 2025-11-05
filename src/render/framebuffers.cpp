#include <iostream>

#include "render/framebuffers.h"
#include "shader/shader.h"


//Screen Framebuffer

Shader screenFramebuffer::shader;
GLuint screenFramebuffer::quadVAO;

void screenFramebuffer::Init()
{
				screenFramebuffer::shader = Shader("assets/shaders/screenShader.vert", "assets/shaders/screenShader.frag");
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				float screenQuadVertices[24] = 
				{
					//position    //texcoords 
					-1.0f, 1.0f,  0.0f, 1.0f,
					-1.0f,-1.0f,  0.0f, 0.0f,
					 1.0f,-1.0f,  1.0f, 0.0f,

					 1.0f,-1.0f,  1.0f, 0.0f,
					 1.0f, 1.0f,  1.0f, 1.0f,
					-1.0f, 1.0f,  0.0f, 1.0f
				};


				glGenVertexArrays(1, &quadVAO);
				GLuint quadVBO;
				glGenBuffers(1, &quadVBO);

				glBindVertexArray(quadVAO);

				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);	

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void screenFramebuffer::Draw(GLuint screenTexture)
{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glDisable(GL_DEPTH_TEST );
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glBindVertexArray(quadVAO);
				shader.use();
				shader.setInt("screenTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, screenTexture);
				glDrawArrays(GL_TRIANGLES, 0, 6);

}


//our main render framebuffer

GLuint renderFramebuffer::FBO;
GLuint renderFramebuffer::textureColorBuffer;
GLuint renderFramebuffer::RBO;

bool renderFramebuffer::Init(int screenWidth, int screenHeight)
{
				glGenFramebuffers(1, &FBO);
				glBindFramebuffer(GL_FRAMEBUFFER, FBO);

				//our Screen Texture
				glGenTextures(1, &textureColorBuffer);
				glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);



				//our render buffer
				glGenRenderbuffers(1, &RBO);
				glBindRenderbuffer(GL_RENDERBUFFER, RBO);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screenWidth, screenHeight);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

				//check renderBufferStatus
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
								std::cerr << "Main Framebuffer completed unsuccesfully, exiting" << std::endl;
								return false;
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				return true;
}


