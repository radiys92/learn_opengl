#pragma once

#include <GL/glew.h>;
#include <vector>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();

private:
	GLuint shaderProgram;
};

typedef struct
{
	GLuint PerVertexCount;
	GLuint Pointer;
} PointerAttributeData;

class Mesh
{
public:
	Mesh(GLenum usage, GLuint verticiesCount);
	void BindVertexArray();
	void AddVertexAttribute(std::vector<GLfloat> attribute, int perVertexCount, int attributePointer);
	void SetIndicesBuffer(std::vector<GLuint> indices);

private:
	GLenum usage;
	GLuint vao;
	GLuint ebo;
	GLuint vbo;

	GLuint verticiesCount;
	std::vector<GLfloat> attributesArray;
	std::vector<GLuint> indices;
	std::vector<PointerAttributeData> vaps;

	void ReinitVertexArray();
};

//class Texture
//{
//public:
//	Texture();
//
//	void LoadFrom(const GLchar* texturePath);
//	int GetWidth();
//	int GetHeight();
//};
//
//
//class Material
//{
//public:
//	Material(const GLchar* vertexPath, const GLchar* fragmentPath);
//
//	void Use();
//};