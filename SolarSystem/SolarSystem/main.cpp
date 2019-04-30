#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ostream>
#include <iostream>

#include "Rendering/Rendering.h"

Shader* shader;
std::vector<Mesh> Figures;

void PrepareEnv()
{
	shader = new Shader("vertex.shader", "fragment.shader");

	Mesh quad = Mesh(GL_STATIC_DRAW, 4);
	quad.AddVertexAttribute(
		std::vector<GLfloat>
	{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	},
		3, 0);
	quad.AddVertexAttribute(
		std::vector<GLfloat>
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		1, 1, 1
	}, 
		3, 1);
	quad.SetIndicesBuffer(
		std::vector<GLuint>
	{
		0, 1, 3,
		1, 2, 3
	});
	Figures.push_back(quad);

	Mesh redTriangle = Mesh(GL_STATIC_DRAW, 3);
	redTriangle.AddVertexAttribute(
		std::vector<GLfloat>
	{
		-0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f
	}, 
		3, 0);
	redTriangle.AddVertexAttribute(
		std::vector<GLfloat>
	{
		0.7f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.3f, 0.0f, 0.0f
	},
		3, 1);
	redTriangle.SetIndicesBuffer(std::vector<GLuint> {0,1,2});
	Figures.push_back(redTriangle);
}

void Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader->Use();

	for (int i = 0; i < Figures.size(); i++)
	{
		Figures[i].BindVertexArray(); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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

	PrepareEnv();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();
		
		glfwSwapBuffers(window);
	}

	return 0;
}
