#include "Rendering.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

SceneNode::SceneNode() { }

SceneNode::SceneNode(SceneNode* parent, SceneObject* object) :
	parent(parent), object(object)
{
}

std::vector<SceneNode*> SceneNode::GetChildrens()
{
	return childrens;
}

SceneNode* SceneNode::AddChild(SceneObject* object)
{
	SceneNode* node = new SceneNode(this, object);
	childrens.push_back(node);
	if (object != nullptr && this->object != nullptr)
	{
		object->GetTransform()->SetParent(this->object->GetTransform());
	}
	return node;
}


void SceneNode::Draw(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors)
{
	if (object != nullptr)
	{
		object->Draw(matrices, vectors);
	}

	for (SceneNode* children : childrens)
	{
		children->Draw(matrices, vectors);
	}
}

void Scene::FillScene()
{
	time_t timeVal;
	srand(static_cast<unsigned>(time(&timeVal)));

	int planetsCount = 5;

	planetBuffer = new EarthPlanetObject();

	sun = new SunObject(planetBuffer->GetMesh());
	Transform* t = sun->GetTransform();
	t->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
	rootNode.AddChild(sun);

	for (int i = 0; i < planetsCount; i++)
	{
		GLfloat radius = i * 2 + 4;
		GLfloat angle = (rand() % 1000) / 1000.0f * glm::pi<GLfloat>() * 2;
		GLfloat x = sin(angle) * radius;
		GLfloat z = cos(angle) * radius;

		EarthPlanetObject* planet = new EarthPlanetObject(planetBuffer->GetMesh(), planetBuffer->GetMaterial());
		planet->SetupOrbitAngle(glm::vec3(0, 0, 0), glm::vec3(0,0.1,0));
		planet->SetParentRotationSpeed((planetsCount - i + 1) * 20);
		Transform *t = planet->GetTransform();
		t->SetPosition(glm::vec3(x, 0, z));
		t->SetScale(glm::vec3(0.7f, 0.7f, 0.7f));
		SceneNode* planetNode = rootNode.AddChild(planet);

		EarthPlanetObject* satellite = new EarthPlanetObject(planetBuffer->GetMesh(), planetBuffer->GetMaterial());
		satellite->SetupOrbitAngle(glm::vec3(0, 0, 0), glm::vec3(0, 0.1, 0));
		satellite->SetParentRotationSpeed((planetsCount - i + 1) * 20);
		t = satellite->GetTransform();
		t->SetPosition(glm::vec3(1.5f, 0, 0));
		t->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		planetNode->AddChild(satellite);
	}
}

SunObject* Scene::GetLightSource()
{
	return sun;
}


void Scene::DrawScene(std::map<const char*, glm::mat4>* matrices, std::map<const char*, glm::vec3>* vectors)
{
	rootNode.Draw(matrices, vectors);
}

void Scene::SetVectorForDefaultPlanet(const char* uniformName, float x, float y, float z)
{
	planetBuffer->SetVector(uniformName, x, y, z);
}
