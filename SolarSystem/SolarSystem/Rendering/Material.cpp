#include "Rendering.h"

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

