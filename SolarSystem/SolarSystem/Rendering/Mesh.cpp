#include "Rendering.h"
#include <iostream>
#include <stdlib.h>

Mesh::Mesh(
	GLenum usage, 
	GLuint verticiesCount)
{
	this->usage = usage;
	this->verticiesCount = verticiesCount;

	this->vao = 0;
	this->ebo = 0;
	this->vbo = 0;
}

void Mesh::AddVertexAttribute(std::vector<GLfloat> attribute, int perVertexCount, int attributePointer)
{
	size_t oldSize = attributesArray.size();
	int newSize = oldSize + verticiesCount * perVertexCount;

	if (newSize != oldSize + attribute.size())
	{
		std::cout << "ERR:MESH:INCORRECT_ATTRIBUTES_ARR_LENGTH" << std::endl
			<< "EXPECTED " << verticiesCount * perVertexCount << std::endl
			<< "BUT WAS " << attribute.size() << std::endl;
		return;
	}

	int vapsSize = vaps.size();
	int oldAttrValuesCount = 0;
	for (int i = 0; i < vapsSize;i++)
	{
		oldAttrValuesCount += vaps[i].PerVertexCount;
	}
	PointerAttributeData vap;
	vap.PerVertexCount = perVertexCount;
	vap.Pointer = attributePointer;
	vaps.push_back(vap);

	std::vector<GLfloat> arr;
	int verticiesIterator = 0;
	int attributeIterator = 0;
	int arrIterator = 0;
	for (int i = 0; i < verticiesCount; i++)
	{
		if (oldAttrValuesCount > 0)
			arr.insert(arr.end(), &attributesArray[attributeIterator], &attributesArray[attributeIterator] + oldAttrValuesCount);
		arrIterator += oldAttrValuesCount;
		attributeIterator += oldAttrValuesCount;
		if (perVertexCount > 0)
			arr.insert(arr.end(), &attribute[verticiesIterator], &attribute[verticiesIterator] + perVertexCount);
		arrIterator += perVertexCount;
		verticiesIterator += perVertexCount;
	}
	attributesArray.clear();
	attributesArray = arr;
//	ReinitVertexArray();
}

void Mesh::SetIndicesBuffer(std::vector<GLuint> value)
{
	indices.clear();
	indices.assign(value.begin(), value.end());
//	ReinitVertexArray();
}

GLsizei Mesh::GetIndicesCount()
{
	return indices.size();
}

void Mesh::BindVertexArray()
{
	if (vao == 0)
	{
		//std::cout << "ERR::MESH::BIND_BREFORE_INIT";
		ReinitVertexArray();
	}
	glBindVertexArray(vao);
}

void Mesh::ReinitVertexArray()
{
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);

	if (indices.size() != 0)
	{
		if (ebo == 0)
		{
			glGenBuffers(1, &ebo);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], usage);
	}
	else if (ebo != 0)
	{
		glDeleteBuffers(1, &ebo);
	}

	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, attributesArray.size() * sizeof(GLfloat), &attributesArray[0], usage);

	int vapsSize = vaps.size();
	int allAttributesCount = 0;
	for (int i = 0; i < vapsSize; i++)
	{
		allAttributesCount += vaps[i].PerVertexCount;
	}

	int offset = 0;
	for (int i = 0; i < vapsSize;i++)
	{
		PointerAttributeData data = vaps[i];
		glVertexAttribPointer(data.Pointer, data.PerVertexCount, GL_FLOAT, GL_FALSE, allAttributesCount * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(data.Pointer);
		offset += data.PerVertexCount;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
