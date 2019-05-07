#include "Rendering.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

OrbitCamera* OrbitCamera::instance = nullptr;

OrbitCamera::OrbitCamera(GLFWwindow* window, GLfloat radius)
{
	instance = this;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	screenAspect = 1.0f * width / height;
	this->radius = radius;
	CalculatePosition();

	for (int i = 0; i < GLFW_KEY_LAST; i++)
		keys[i] = false;
	glfwSetKeyCallback(window, key_callback);
}

void OrbitCamera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < GLFW_KEY_LAST)
	{
		if (action == GLFW_PRESS)
			instance->keys[key] = true;
		else if (action == GLFW_RELEASE)
			instance->keys[key] = false;
	}
}

void OrbitCamera::CalculatePosition()
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians(rotation.x), glm::vec3(0, 1, 0));
	rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(1, 0, 0));
	position = rot * glm::vec4(0, 0, radius, 1);
}

glm::vec3 OrbitCamera::GetViewPosition()
{
	return position;
}

glm::mat4 OrbitCamera::GetViewMatrix()
{
	return glm::lookAt(position, targetPos, up);
}

glm::mat4 OrbitCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), screenAspect, nearPlane, farPlane);
}

glm::vec3 OrbitCamera::GetRight()
{
	return glm::normalize(glm::cross(targetPos - position, up));
}


void OrbitCamera::Update()
{
	DeltaTimeHolder::GetInstance().Update();

	GLfloat cameraSpeed = 35.0f * DeltaTimeHolder::GetInstance().deltaTime;
	// cam pos
	if (keys[GLFW_KEY_W])
		rotation.y = glm::clamp(rotation.y + cameraSpeed, 180.0f, 270.0f);
	if (keys[GLFW_KEY_S])
		rotation.y = glm::clamp(rotation.y - cameraSpeed, 180.0f, 270.0f);;
	if (keys[GLFW_KEY_A])
		rotation.x -= cameraSpeed;
	if (keys[GLFW_KEY_D])
		rotation.x += cameraSpeed;

	// fov
	if (keys[GLFW_KEY_Q])
		fov = glm::clamp(fov - cameraSpeed, 10.0f, 90.0f);
	if (keys[GLFW_KEY_E])
		fov = glm::clamp(fov + cameraSpeed, 10.0f, 90.0f);

	// streif
	if (keys[GLFW_KEY_LEFT])
		targetPos = targetPos - GetRight() * cameraSpeed / 2.0f;
	if (keys[GLFW_KEY_RIGHT])
		targetPos = targetPos + GetRight() * cameraSpeed / 2.0f;

	CalculatePosition();
}
