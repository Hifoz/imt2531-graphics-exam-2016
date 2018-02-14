#version 410

layout(location = 0) in vec3 position;

out vec3 texcoord;

uniform mat4 VP;


void main()
{
	gl_Position = VP * vec4(position, 1.0);
	texcoord = position;
}