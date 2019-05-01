#include "Rendering.h"
#include <SOIL.h>
#include <iostream>

Texture2D::Texture2D(const GLchar* uniformName, int textureIndex)
{
	this->uniformName = uniformName;
	this->textureIndex = textureIndex;
}

void Texture2D::LoadFrom(const GLchar* texturePath)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int width, height, c;
	unsigned char* data = SOIL_load_image(texturePath, &width, &height, &c, SOIL_LOAD_RGB);
	if (data == 0)
	{
		std::cout << "TEXTURE2D::LOAD_ERROR" << std::endl << SOIL_last_result() << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture2D::BindTo(GLuint program)
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(program, uniformName), textureIndex);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}