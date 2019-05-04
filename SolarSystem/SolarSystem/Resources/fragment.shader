#version 330 core
in vec4 VertexColor;
in vec2 TexCoord;
in vec2 CloudsTexCoord;

out vec4 color;

uniform sampler2D mainTex;
uniform sampler2D cloudsTex;

void main()
{
    // color = VertexColor * texture(mainTex, TexCoord);
    color = texture(mainTex, TexCoord);
    color += texture(cloudsTex, CloudsTexCoord) / 2;
} 