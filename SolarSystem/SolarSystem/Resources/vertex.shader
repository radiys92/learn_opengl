#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec4 VertexColor;
out vec2 TexCoord;
out vec2 CloudsTexCoord;

uniform mat4 MVP_Matrix;
uniform vec2 cloudsShift;

void main()
{
    gl_Position = MVP_Matrix * vec4(position, 1.0);
    VertexColor = vec4(color, 1.0);
    TexCoord = texCoord;
    CloudsTexCoord = texCoord + cloudsShift;
}