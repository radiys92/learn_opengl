#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering.h"

glm::mat4 identity(1.0f);
glm::vec3 up(0, 1, 0), right(1, 0, 0), forward(0, 0, 1);

glm::vec3 Transform::GetPosition()
{
	return position;
}

void Transform::SetPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 Transform::GetRotation()
{
	return rotation;
}

void Transform::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}


glm::vec3 Transform::GetScale()
{
	return scale;
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), right);
	model = glm::rotate(model, glm::radians(rotation.y), up);
	model = glm::rotate(model, glm::radians(rotation.z), forward);
	model = glm::scale(model, scale);

	Transform* p = parent;
	while (p != nullptr)
	{
		model = p->GetModelMatrix() * model;
		p = p->GetParent();
	}

	return model;
}

Transform* Transform::GetParent()
{
	return parent;
}

void Transform::SetParent(Transform* parent)
{
	this->parent = parent;
}