#pragma once

#include <GL/glew.h>;
#include <vector>

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

class Texture2D
{
public:
	Texture2D(const GLchar* uniformName, int textureIndex);
	void LoadFrom(const GLchar* texturePath);
	void BindTo(GLuint program);
	void Unbind();

private:
	const GLchar* uniformName;
	int textureIndex;
	GLuint textureId;
};

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	GLuint GetProgramId();

	void Use();
private:
	GLuint shaderProgram;
};

class Material
{
public:
	void LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void LoadTexture2D(const GLchar* texturePath, const GLchar* uniformName);

	void Use();
	void Unbind();

private:
	Shader* shader;
	std::vector<Texture2D> textures;
};