#pragma once

#include <string>
#include "glad/glad.h"
#include "stb_image.h"

GLuint loadImageFromFile(std::string imagePath, bool flipImage);
