#pragma once

#include <GL/glew.h>;
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x2.hpp>
#include <map>
#include <GLFW/glfw3.h>
#include <SOIL.h>

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
	void LoadFrom(const GLchar* texturePath, int soilLoadMode);
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
	void LoadTexture2D(const GLchar* texturePath, const GLchar* uniformName, int soilLoadMode);

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

	Transform* GetParent();
	void SetParent(Transform* parent);

private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	Transform* parent = NULL;
};

class SceneObject
{
public:
	SceneObject(Mesh* mesh, Material* mat)
		: mesh(mesh), material(mat)
	{
	}

	Transform* GetTransform();

	void SetMaterial(Material* material);
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();

	void SetVector(const char* uniformName, GLfloat x, GLfloat y);
	void SetVector(const char* uniformName, GLfloat x, GLfloat y, GLfloat z);

	virtual void Draw(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors);
	Material* GetMaterial();
private:
	Transform transfom;
	Material *material;
	Mesh *mesh;
};

class SphereSceneObject : public SceneObject
{
public:
	SphereSceneObject(Material* mat) : SceneObject(GenerateIdentitySphere(15), mat) { }
	SphereSceneObject(Mesh* mesh, Material* mat) : SceneObject(mesh, mat) { }

private:
	Mesh* GenerateIdentitySphere(int segments);
};

class SunObject : public SphereSceneObject
{
public:
	SunObject() : SphereSceneObject(GenerateMaterial()) { }
	SunObject(Mesh* mesh) : SphereSceneObject(mesh, GenerateMaterial()) { }

	glm::vec3 GetLightColor();

	Material* GenerateMaterial()
	{
		Material* mat = new Material();
		mat->LoadShader("Resources/default_vertex.shader", "Resources/default_fragment.shader");
		mat->LoadTexture2D("Resources/Sun_Alb.png", "mainTex", SOIL_LOAD_RGB);
		return mat;
	}
private:
	glm::vec3 lightColor = glm::vec3(1.0f, 0.96f, 0.76f);;
};

class EarthPlanetObject : public SphereSceneObject
{
public:
	EarthPlanetObject() : SphereSceneObject(GenerateMaterial()) { }
	EarthPlanetObject(Mesh* mesh, Material* mat) : SphereSceneObject(mesh, mat) { }

	Material* GenerateMaterial()
	{
		Material* mat = new Material();
		mat->LoadShader("Resources/multimaps_vertex.shader", "Resources/multimaps_fragment.shader");
		mat->LoadTexture2D("Resources/Earth_Alb.png", "mainTex", SOIL_LOAD_RGB);
		mat->LoadTexture2D("Resources/Earth Clouds.png", "cloudsTex", SOIL_LOAD_L);
		mat->LoadTexture2D("Resources/Earth Lights.png", "lightsTex", SOIL_LOAD_RGB);
		mat->LoadTexture2D("Resources/Earth Ocean.png", "specularTex", SOIL_LOAD_L);
		return mat;
	}

	virtual void Draw(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors) override;
	void SetupOrbitAngle(glm::vec3 orbitAngle, glm::vec3 rotationSped);
	void SetParentRotationSpeed(GLfloat speed);

private:
	glm::vec3 orbitRotation;
	GLfloat cloudsSpeed = -150.0f;
	glm::vec3 orbitRotationSpeed;
	GLfloat parentRotationSpeed = 10;
};

class SceneNode
{
public:
	SceneNode();
	SceneNode(SceneNode* parent, SceneObject* object);

	std::vector<SceneNode*> GetChildrens();
	SceneNode* AddChild(SceneObject* object);

	void Draw(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors);

private:
	std::vector<SceneNode*> childrens;
	SceneNode* parent;
	SceneObject* object;
};

class Scene
{
public:
	void SetVectorForDefaultPlanet(const char* uniformName, float x, float y, float z);
	void FillScene();
	SunObject* GetLightSource();
	void DrawScene(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors);

private:
	SceneNode rootNode = SceneNode();
	SunObject* sun;
	EarthPlanetObject* planetBuffer;
};

class OrbitCamera
{
public:
	OrbitCamera(GLFWwindow* window, GLfloat radius);

	void Update();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetViewPosition();

private:
	glm::vec3 position;
	glm::vec3 targetPos = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	GLfloat fov = 45.0f;
	GLfloat nearPlane = 0.1f;
	GLfloat farPlane = 100.0f;
	GLfloat screenAspect;
	GLfloat radius;
	glm::vec2 rotation = glm::vec2(0, 235);

	void CalculatePosition();
	glm::vec3 GetRight();

	bool keys[GLFW_KEY_LAST];
	static OrbitCamera* instance;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
};

class DeltaTimeHolder
{
public:
	GLfloat deltaTime;
	GLfloat lastFrame;

	static DeltaTimeHolder& GetInstance()
	{
		static DeltaTimeHolder instance;
		return instance;
	}

	void Update()
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
};