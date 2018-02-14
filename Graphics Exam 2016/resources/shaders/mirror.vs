#version 410

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 texcoord;

uniform mat4 MVP;

out vec2 TexCoords;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
    TexCoords = texcoord;
}