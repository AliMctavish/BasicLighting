#version 330 core
layout (location = 0) in vec3 vecPos;
layout (location = 1) in vec3 colorVal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 ourTexture;

void main()
{
gl_Position =  projection * view * model * vec4(vecPos.x,vecPos.y, vecPos.z ,1);
ourColor = colorVal;
}