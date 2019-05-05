#include "Rendering.h"
#include <glm/gtc/type_ptr.inl>

void Material::LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	shader = new Shader(vertexPath, fragmentPath);
}

void Material::LoadTexture2D(const GLchar* texturePath, const GLchar* uniformName)
{
	Texture2D tex = Texture2D(uniformName, textures.size());
	tex.LoadFrom(texturePath);
	textures.insert(textures.end(), tex);
}

void Material::Use()
{
	shader->Use();
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].BindTo(shader->GetProgramId());
	}
}

void Material::Unbind()
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].Unbind();
	}
	glUseProgram(0);
}

Shader* Material::GetShader()
{
	return  shader;
}

void Material::SetMatrix(const char* uniformName, glm::mat4 mat4)
{
	GLuint uniformLocation = glGetUniformLocation(shader->GetProgramId(), uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
}
