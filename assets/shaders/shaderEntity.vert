#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

layout (std140) uniform Matrices
{
        mat4 projection;
        mat4 view;
};

uniform mat4 model;
uniform mat3 normalCalculationMatrix;

out vec3 fragPos;
out vec3 normalVector;
out vec2 TexCoord;

void main()
{
        gl_Position = projection * view * model * vec4(Position, 1.0f);

        fragPos = vec3(model * vec4(Position, 1.0f));
        normalVector = normalCalculationMatrix * Normal;
        TexCoord = TexCoords;
}
