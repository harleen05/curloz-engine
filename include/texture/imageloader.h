#pragma once

#include <string>
#include "glad/glad.h"
#include "stb_image.h"

class Texture 
{
	public:
		static GLuint loadImageFromFile(std::string imagePath, bool flipImage);
};
