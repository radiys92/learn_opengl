#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ostream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Rendering.h"
#include <ctime>

std::vector<SceneObject*> objects;
SunObject* Sun;

void PrepareScene()
{
	time_t timeVal;
	srand(static_cast<unsigned>(time(&timeVal)));

	glm::vec3 sunPos(0.0f, 0.0f, 0.0f);
	int planetsCount = 5;

	for (int i = 0; i < planetsCount; i++)
	{
		GLfloat radius = i * 2 + 4;
		GLfloat angle = (rand() % 1000) / 1000.0f * glm::pi<GLfloat>() * 2;
		GLfloat x = sin(angle) * radius;
		GLfloat z = cos(angle) * radius;

		EarthPlanetObject* planet;
		if (objects.size() > 0)
			planet = new EarthPlanetObject(objects[0]->GetMesh(), objects[0]->GetMaterial());
		else
			planet = new EarthPlanetObject();
		Transform *t = planet->GetTransform();
		glm::vec3 planetRotation(25, 0, 0);
		t->SetPosition(glm::vec3(x, 0, z));
		t->SetRotation(planetRotation);
		t->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
		objects.push_back(planet);
	}

	for (int i = 0; i < planetsCount; i++)
	{
		EarthPlanetObject* earth = new EarthPlanetObject(objects[0]->GetMesh(), objects[0]->GetMaterial());
		Transform *t = earth->GetTransform();
		t->SetPosition(glm::vec3(1.5f, 0, 0));
		t->SetRotation(glm::vec3(25, 0, 0));
		t->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		t->SetParent(objects[i]->GetTransform());
		objects.push_back(earth);
	}

	SunObject* sun = new SunObject(objects[0]->GetMesh());
	Transform* t = sun->GetTransform();
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

	glm::vec3 viewPos(0.0f, 40.0, -5.0f);
	glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	Sun->SetViewMatrix(view);
	Sun->SetProjectionMatrix(projection);
	Sun->Draw();

	glm::vec3 lightPos = Sun->GetTransform()->GetPosition();
	for (SceneObject* object : objects)
	{
		object->SetViewMatrix(view);
		object->SetProjectionMatrix(projection);
		object->SetVector("lightPos", lightPos.x, lightPos.y, lightPos.z);
		object->SetVector("viewPos", viewPos.x, viewPos.y, viewPos.z);
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
