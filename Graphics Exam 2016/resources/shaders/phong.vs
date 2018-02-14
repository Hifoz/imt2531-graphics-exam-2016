#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 MVP;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 frag_color;
out vec3 frag_pos;
out vec3 frag_normal;

void main () {
	gl_Position = MVP * vec4(position, 1.0);

	frag_color = color;
	frag_pos = vec3(ModelMatrix * vec4(position, 1.0));
	frag_normal = mat3(transpose(inverse(ModelMatrix))) * normal;
}