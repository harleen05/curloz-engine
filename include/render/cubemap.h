#pragma once

#include <string>
#include "glad/glad.h"
#include "shader/shader.h"
#include "global/globalConfig.h"

class CubeMap
{
	public:
		/**
		 * the array stores cubemaps like this ->
		 * 0 = Right
		 * 1 = Left
		 * 2 = Top
		 * 3 = Bottom
		 * 4 = Front
		 * 5 = Back
		 */
		static void init();
		static void Draw();

		static GLuint VAO;
		static GLuint CUBE_MAP;
		static Shader shader;
};
