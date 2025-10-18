#version 330 core
layout (location = 0) in vec3 Position;

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoords;

void main()
{
	TexCoords = Position;
	vec4 pos = projection * view * vec4(Position, 1.0);
	gl_Position = pos.xyww;
}
