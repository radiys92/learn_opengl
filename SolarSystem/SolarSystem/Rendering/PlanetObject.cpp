#include "Rendering.h"
#include <GLFW/glfw3.h>

Mesh* SphereSceneObject::GenerateIdentitySphere(int segments = 10)
{
	float M_PI = 3.1415f;
	uint32_t verticesCount = 0;
	int parallels = segments, meridians = segments;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> uvs;
	for (uint32_t j = 0; j < parallels + 1; ++j)
	{
		GLfloat polar = M_PI * GLfloat(j) / GLfloat(parallels);
		GLfloat sp = std::sin(polar);
		GLfloat cp = std::cos(polar);
		for (uint32_t i = 0; i < meridians + 1; ++i)
		{
			GLfloat azimuth = 2.0 * M_PI * GLfloat(i) / GLfloat(meridians);
			GLfloat sa = std::sin(azimuth);
			GLfloat ca = std::cos(azimuth);
			GLfloat x = sp * ca;
			GLfloat y = cp;
			GLfloat z = sp * sa;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			glm::vec3 normal = normalize(glm::vec3(x, y, z));
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
			GLfloat u = static_cast<GLfloat>(j) / parallels;
			GLfloat v = static_cast<GLfloat>(i) / meridians;
			uvs.push_back(v);
			uvs.push_back(u);
			verticesCount++;
		}
	}

	std::vector<GLuint> indices;
	for (uint32_t j = 0; j <= parallels; ++j)
	{
		GLuint aStart = j * meridians + 1;
		GLuint bStart = (j + 1) * meridians + 1;
		for (uint32_t i = 0; i < meridians; ++i)
		{
			GLuint a = aStart + i;
			GLuint a1 = aStart + (i + 1) % (meridians + 1);
			GLuint b = bStart + i;
			GLuint b1 = bStart + (i + 1) % (meridians + 1);
			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);
		}
	}

	Mesh* sphere = new Mesh(GL_STATIC_DRAW, verticesCount);
	sphere->AddVertexAttribute(vertices, 3, 0);
	sphere->AddVertexAttribute(uvs, 2, 1);
	sphere->AddVertexAttribute(normals, 3, 2);
	sphere->SetIndicesBuffer(indices);
	return sphere;
}

void EarthPlanetObject::Draw()
{
	this->SetVector("cloudsShift", (GLfloat)glfwGetTime() / -150.0f, 0);
	SceneObject::Draw();
}

glm::vec3 SunObject::GetLightColor()
{
	return lightColor;
}