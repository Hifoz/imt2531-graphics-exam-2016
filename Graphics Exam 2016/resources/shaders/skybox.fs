#version 410

in vec3 texcoord;

out vec4 fragment_color;

uniform samplerCube skybox;
uniform bool isNight;

void main()
{
	fragment_color = texture(skybox, texcoord).zyxw;	// .zyxq because .bmp has red and blue channels swapped
	if(isNight)
		fragment_color *= 0.3;
}