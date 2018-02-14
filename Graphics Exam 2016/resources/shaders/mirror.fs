#version 410

in vec2 TexCoords;

out vec4 fragment_color;

uniform sampler2D mirrorTexture;

void main()
{ 
    fragment_color = texture(mirrorTexture, TexCoords);
}