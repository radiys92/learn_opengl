#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ostream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Rendering.h"

Material Material1;
std::vector<Mesh> Figures;

Mesh GenerateIdentitySphere(int segments = 10)
{
	float M_PI = 3.1415f;
	uint32_t verticesCount = 0;
	int parallels = segments, meridians = segments;
	std::vector<GLfloat> vertices;
//	std::vector<GLfloat> colors;
	std::vector<GLfloat> uvs;
	for (uint32_t j = 0; j < parallels+1; ++j)
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
			GLfloat u = static_cast<GLfloat>(j) / (parallels - 1);
			GLfloat v = static_cast<GLfloat>(i) / meridians;
			uvs.push_back(v);
			uvs.push_back(u);
			verticesCount++;

//			colors.push_back(0);
//			colors.push_back(1);
//			colors.push_back(0);
		}
	}

	std::vector<GLuint> indices;
	for (uint32_t j = 0; j < parallels; ++j)
	{
		GLuint aStart = j * meridians + 1;
		GLuint bStart = (j + 1) * meridians + 1;
		for (uint32_t i = 0; i < meridians; ++i)
		{
			GLuint a = aStart + i;
			GLuint a1 = aStart + (i + 1) % (meridians+1);
			GLuint b = bStart + i;
			GLuint b1 = bStart + (i + 1) % (meridians+1);
			indices.push_back(a);
			indices.push_back(a1);
			indices.push_back(b1);
			indices.push_back(a);
			indices.push_back(b1);
			indices.push_back(b);
		}
	}

	for (uint32_t i = 0; i < meridians; ++i)
	{
		uint32_t const a = i + meridians * (parallels - 2) + 1;
		uint32_t const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
		indices.push_back(verticesCount - 1);
		indices.push_back(a);
		indices.push_back(b);
	}


	Mesh sphere(GL_STATIC_DRAW, verticesCount);
	sphere.AddVertexAttribute(vertices, 3, 0);
//	sphere.AddVertexAttribute(colors, 3, 1);
	sphere.AddVertexAttribute(uvs, 2, 2);
	sphere.SetIndicesBuffer(indices);
	return sphere;
}

void PrepareEnv()
{
	Material1.LoadShader("Resources/vertex.shader", "Resources/fragment.shader");
	Material1.LoadTexture2D("Resources/Earth_Alb.png", "mainTex");
	Material1.LoadTexture2D("Resources/Earth Clouds.png", "cloudsTex");
//	Material1.LoadTexture2D("Resources/tileable-grass_clover_TT7010116_nm.png", "normal");

	Mesh sphere = GenerateIdentitySphere(15);
	Figures.push_back(sphere);

	glEnable(GL_DEPTH_TEST);
}

void Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//	glPointSize(5);

	Material1.Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0));
	model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.1, 0.1, 0.0));
	model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0, 0, -10));
	view = glm::rotate(view, glm::radians(15.0f), glm::vec3(1, 0, 0));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 30.0f);

	glm::mat4 mvp = projection * view * model;

	GLuint program = Material1.GetShader()->GetProgramId();
	GLuint uniformLoc = glGetUniformLocation(program, "MVP_Matrix");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform2f(glGetUniformLocation(program, "cloudsShift"), (GLfloat)glfwGetTime() / -150.0f, 0);


	for (int i = 0; i < Figures.size(); i++)
	{
		Figures[i].BindVertexArray(); 
		glDrawElements(GL_TRIANGLES, Figures[i].GetIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	Material1.Unbind();
}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
		" type = " << type << ", severity = " << severity << ", message = " << message << "\n";
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "SolarSystem", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	PrepareEnv();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw();
		
		glfwSwapBuffers(window);
	}

	return 0;
}
