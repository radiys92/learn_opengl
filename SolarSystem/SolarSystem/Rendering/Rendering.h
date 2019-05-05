#pragma once

#include <GL/glew.h>;
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>

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
	GLsizei GetIndicesCount();

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
	Shader* GetShader();
	void SetMatrix(const char* str, glm::mat4 mat4);

private:
	Shader* shader;
	std::vector<Texture2D> textures;
};

class Transform
{
public:
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 pos);

	glm::vec3 GetRotation();
	void SetRotation(glm::vec3 rotation);

	glm::vec3 GetScale();
	void SetScale(glm::vec3 scale);

	glm::mat4 GetModelMatrix();

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
};

class SceneObject
{
public:
	explicit SceneObject(Mesh* mesh)
		: mesh(mesh)
	{
	}

	Transform* GetTransform();

	void SetMaterial(Material material);
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();

	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetVector(const char* uniformName, GLfloat x, GLfloat y);
	void SetVector(const char* uniformName, GLfloat x, GLfloat y, GLfloat z);

	virtual void Draw();
private:
	Transform transfom;
	Material material;
	Mesh *mesh;
};

class SphereSceneObject : public SceneObject
{
public:
	SphereSceneObject() : SceneObject(GenerateIdentitySphere(15)) { }
	SphereSceneObject(Mesh* mesh) : SceneObject(mesh) { }

private:
	Mesh* GenerateIdentitySphere(int segments);
};

class SunObject : public SphereSceneObject
{
public:
	SunObject() : SphereSceneObject()
	{
		Material mat;
		mat.LoadShader("Resources/default_vertex.shader", "Resources/default_fragment.shader");
		mat.LoadTexture2D("Resources/Sun_Alb.png", "mainTex");
		SetMaterial(mat);
	}

	SunObject(Mesh* mesh) : SphereSceneObject(mesh)
	{
		Material mat;
		mat.LoadShader("Resources/default_vertex.shader", "Resources/default_fragment.shader");
		mat.LoadTexture2D("Resources/Sun_Alb.png", "mainTex");
		SetMaterial(mat);
	}

	glm::vec3 GetLightColor();

private:
	glm::vec3 lightColor = glm::vec3(1.0f, 0.96f, 0.76f);;
};

class EarthPlanetObject : public SphereSceneObject
{
public:
	EarthPlanetObject() : SphereSceneObject()
	{
		Material mat;
		mat.LoadShader("Resources/multimaps_vertex.shader", "Resources/multimaps_fragment.shader");
		mat.LoadTexture2D("Resources/Earth_Alb.png", "mainTex");
		mat.LoadTexture2D("Resources/Earth Clouds.png", "cloudsTex");
		mat.LoadTexture2D("Resources/Earth Lights.png", "lightsTex");
		SetMaterial(mat);
	}

	EarthPlanetObject(Mesh* mesh) : SphereSceneObject(mesh)
	{
		Material mat;
		mat.LoadShader("Resources/multimaps_vertex.shader", "Resources/multimaps_fragment.shader");
		mat.LoadTexture2D("Resources/Earth_Alb.png", "mainTex");
		mat.LoadTexture2D("Resources/Earth Clouds.png", "cloudsTex");
		mat.LoadTexture2D("Resources/Earth Lights.png", "lightsTex");
		SetMaterial(mat);
	}

	void Draw();
};