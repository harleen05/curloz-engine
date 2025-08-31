#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat3 normalMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];


out vec3 FragPos;
out vec3 normalVector;
out vec2 TexCoords;


void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    for(int i=0; i<MAX_BONE_INFLUENCE; ++i)
    {
        if(boneIDs[i] == -1) continue;
        if(boneIDs[i] >= MAX_BONES)
        {
            totalPosition = vec4(Position, 1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIDs[i]] * vec4(Position, 1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIDs[i]]) * Normal;
        totalNormal += localNormal * weights[i];
    }
    gl_Position = projection * view * model * totalPosition;
    FragPos = vec3(model * totalPosition);
    normalVector = normalMatrix * totalNormal; /*highly performance expensive*/
    TexCoords = TexCoord;
}
