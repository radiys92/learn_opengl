#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D mainTex;

void main()
{
    color = texture(mainTex, TexCoord);
} 