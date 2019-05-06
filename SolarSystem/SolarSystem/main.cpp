#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ostream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Rendering.h"
#include <map>

const char* viewMatrixName = "view";
const char* projectionMatrixName = "projection";
const char* lightColorName = "lightColor";
const char* lightPosName = "lightPos";
const char* viewPosName = "viewPos";

Scene scene;

void PrepareScene()
{
	glEnable(GL_DEPTH_TEST);
	scene.FillScene();
}

void Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec3 viewPos(25.0f, 10.0, -5.0f);
	glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	std::map<const char*, glm::mat4> matrices;
	matrices.insert_or_assign(viewMatrixName, view);
	matrices.insert_or_assign(projectionMatrixName, projection);

	std::map<const char*, glm::vec3> vectors;
	vectors.insert_or_assign(lightColorName, scene.GetLightSource()->GetLightColor());
	vectors.insert_or_assign(lightPosName, scene.GetLightSource()->GetTransform()->GetPosition());
	vectors.insert_or_assign(viewPosName, viewPos);

	scene.DrawScene(&matrices, &vectors);
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

	PrepareScene();

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
