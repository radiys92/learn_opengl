#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ostream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Rendering.h"

std::vector<SceneObject*> objects;
SunObject* Sun;

void PrepareScene()
{
	EarthPlanetObject* earth = new EarthPlanetObject();
	Transform *t = earth->GetTransform();
	t->SetPosition(glm::vec3(5.0f, 0, 0));
	t->SetRotation(glm::vec3(25, 0, 0));
	t->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
	objects.push_back(earth);

	SunObject* sun = new SunObject(earth->GetMesh());
	t = sun->GetTransform();
	t->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

	Sun = sun;
	glm::vec3 color = sun->GetLightColor();
	for (SceneObject* object : objects)
	{
		object->SetVector("lightColor", color.r, color.g, color.b);
	}

	glEnable(GL_DEPTH_TEST);
}

void Draw()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat radius = 15.0f;
	GLfloat t = glfwGetTime() / 3 + 3.1415f*2/3;
	GLfloat camX = sin(t) * radius;
	GLfloat camZ = cos(t) * radius;
	glm::mat4 view(1.0f);
	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 30.0f);

	Sun->SetViewMatrix(view);
	Sun->SetProjectionMatrix(projection);
	Sun->Draw();

	glm::vec3 lightPos = Sun->GetTransform()->GetPosition();
	for (SceneObject* object : objects)
	{
		object->SetViewMatrix(view);
		object->SetProjectionMatrix(projection);
		object->SetVector("lightPos", lightPos.x, lightPos.y, lightPos.z);
		object->SetVector("viewPos", camX, 0, camZ);
		object->Draw();
	}
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
