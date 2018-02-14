#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 MVP;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 frag_color;
out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;

void main () {
	gl_Position = MVP * vec4(position, 1.0);

	frag_pos = vec3(ModelMatrix * vec4(position, 1.0));
	frag_color = color;
	frag_normal = mat3(transpose(inverse(ModelMatrix))) * normal;
	frag_texcoord = texcoord;
}