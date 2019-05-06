#include "Rendering.h"
#include <map>
#include <glm/gtc/type_ptr.hpp>

const char* modelMatrixName = "model";

Transform* SceneObject::GetTransform()
{
	return &transfom;
}

void SceneObject::SetMaterial(Material* material)
{
	this->material = material;
}

Material* SceneObject::GetMaterial()
{
	return material;
}


void SceneObject::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* SceneObject::GetMesh()
{
	return mesh;
}

void SceneObject::SetVector(const char* uniformName, GLfloat x, GLfloat y)
{
	material->GetShader()->Use();
	glUniform2f(glGetUniformLocation(material->GetShader()->GetProgramId(), uniformName), x, y);
}

void SceneObject::SetVector(const char* uniformName, GLfloat x, GLfloat y, GLfloat z)
{
	material->GetShader()->Use();
	glUniform3f(glGetUniformLocation(material->GetShader()->GetProgramId(), uniformName), x, y, z);
}

void SceneObject::Draw(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors)
{
	material->Use();

	GLuint uniformModelLocation = glGetUniformLocation(material->GetShader()->GetProgramId(), modelMatrixName);
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(transfom.GetModelMatrix()));

	for (std::map<const char*, glm::mat4>::iterator matrix = matrices->begin(); matrix != matrices->end(); ++matrix)
	{
		GLint pos = glGetUniformLocation(material->GetShader()->GetProgramId(), matrix->first);
		if (pos != -1)
			glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(matrix->second));
	}
	for (std::map<const char*, glm::vec3>::iterator vector = vectors->begin(); vector != vectors->end(); ++vector)
	{
		GLint pos = glGetUniformLocation(material->GetShader()->GetProgramId(), vector->first);
		if (pos != -1)
			glUniform3f(pos, vector->second.x, vector->second.y, vector->second.z);
	}

	mesh->BindVertexArray();
	glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	material->Unbind();
}