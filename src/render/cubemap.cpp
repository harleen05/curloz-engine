#include <iostream>
#include "render/cubemap.h"
#include "global/globalConfig.h"
#include "texture/stb_image.h"
#include "global/globalCamera.h"
#include "toml/toml.hpp"

GLuint CubeMap::VAO;
GLuint CubeMap::CUBE_MAP;
Shader CubeMap::shader;


void CubeMap::init()
{
	shader = Shader("assets/shaders/cubeMap.vert", "assets/shaders/cubeMap.frag");

	glGenTextures(1, &CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CUBE_MAP);

	int mapWidth, mapHeight, nrChannels;
	unsigned char *data;

	toml::table &config = global::Config::getConfig();

	const char *MapArray[6] = 
	{
		config["cubemap"]["right"].value_or("lol"),
		config["cubemap"]["left"].value_or("lol"),
		config["cubemap"]["top"].value_or("lol"),
		config["cubemap"]["bottom"].value_or("lol"),
		config["cubemap"]["front"].value_or("lol"),
		config["cubemap"]["back"].value_or("lol"),
	};

	for( GLuint i=0; i<6 ; ++i )
	{
		data = stbi_load(MapArray[i], &mapWidth, &mapHeight, &nrChannels, 0);

		GLenum format;
		if (nrChannels == 1) format = GL_RED;	
		if (nrChannels == 3) format = GL_RGB;	
		if (nrChannels == 4) format = GL_RGBA;	

		if (!data)
		{
			std::cerr << "Could not load cubemap" << std::endl;
		}
		else 
		{
			glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, format, mapWidth, mapHeight, 0, format, GL_UNSIGNED_BYTE, data
				    );
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	      float CubeMapVertices[108] = {
		      // positions
		-1.0f,  1.0f, -1.0f,
		      -1.0f, -1.0f, -1.0f,
		       1.0f, -1.0f, -1.0f,
		       1.0f, -1.0f, -1.0f,
		       1.0f,  1.0f, -1.0f,
		      -1.0f,  1.0f, -1.0f,

		      -1.0f, -1.0f,  1.0f,
		      -1.0f, -1.0f, -1.0f,
		      -1.0f,  1.0f, -1.0f,
		      -1.0f,  1.0f, -1.0f,
		      -1.0f,  1.0f,  1.0f,
		      -1.0f, -1.0f,  1.0f,

		       1.0f, -1.0f, -1.0f,
		       1.0f, -1.0f,  1.0f,
		       1.0f,  1.0f,  1.0f,
		       1.0f,  1.0f,  1.0f,
		       1.0f,  1.0f, -1.0f,
		       1.0f, -1.0f, -1.0f,

		      -1.0f, -1.0f,  1.0f,
		      -1.0f,  1.0f,  1.0f,
		       1.0f,  1.0f,  1.0f,
		       1.0f,  1.0f,  1.0f,
		       1.0f, -1.0f,  1.0f,
		      -1.0f, -1.0f,  1.0f,

		      -1.0f,  1.0f, -1.0f,
		       1.0f,  1.0f, -1.0f,
		       1.0f,  1.0f,  1.0f,
		       1.0f,  1.0f,  1.0f,
		      -1.0f,  1.0f,  1.0f,
		      -1.0f,  1.0f, -1.0f,

		      -1.0f, -1.0f, -1.0f,
		      -1.0f, -1.0f,  1.0f,
		       1.0f, -1.0f, -1.0f,
		       1.0f, -1.0f, -1.0f,
		      -1.0f, -1.0f,  1.0f,
		       1.0f, -1.0f,  1.0f
	      };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMapVertices), CubeMapVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void CubeMap::Draw()
{		
	glDepthFunc(GL_LEQUAL);
	shader.use();
	shader.setInt("CubeMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CUBE_MAP);
	shader.setMat4("projection", global::CameraManager::projection);
	shader.setMat4("view", glm::mat4(glm::mat3(global::CameraManager::view)));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

}
