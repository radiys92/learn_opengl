#version 330 core
in vec4 VertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D mainTex;
uniform sampler2D normal;

void main()
{
    color = VertexColor ;//* texture(mainTex, TexCoord);
} 