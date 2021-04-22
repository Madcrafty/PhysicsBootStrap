#include "Scene.h"
#include "Camera.h"
#include "Instance.h"
#include <Gizmos.h>

// Constructors
Scene::Scene(glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight)
{
	m_activeCamera = new Camera("FreeCam", true);
	AddCamera(m_activeCamera);
}
Scene::Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_activeCamera(a_camera), m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight)
{
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		delete *i;
	}
}

// Add Instance of an object to the scene
void Scene::AddInstance(Instance* a_instance)
{
	// Check if name exists in list
	bool nametaken = true;
	std::string tmpName = a_instance->GetName();
	int index = 1;
	while (nametaken == true)
	{
		nametaken = false;
		// Get list of exisiting objects
		for (auto const& obj : GetInstances())
		{
			// if there is an object with the same name then change name of new object
			if (obj->GetName() == tmpName)
			{
				nametaken = true;
				tmpName = a_instance->GetName();
				tmpName += " (";
				tmpName += std::to_string(index);
				tmpName += ")";
				index++;
			}
		}
	}
	a_instance->SetName(tmpName);
	m_instances.push_back(a_instance);
}

// Add a Camera to the scene
void Scene::AddCamera(Camera* a_camera)
{
	// Check if name exists in list
	bool nametaken = true;
	std::string tmpName = a_camera->GetName();
	int index = 1;
	while (nametaken == true)
	{
		nametaken = false;
		// Get list of exisiting objects
		for (auto const& obj : m_cameras)
		{
			// if there is an object with the same name then change name of new object
			if (obj->GetName() == tmpName)
			{
				nametaken = true;
				tmpName = a_camera->GetName();
				tmpName += " (";
				tmpName += std::to_string(index);
				tmpName += ")";
				index++;
			}
		}
	}
	a_camera->SetName(tmpName);
	m_cameras.push_back(a_camera);
}

// Add a Light to the scene
void Scene::AddLight(Light* a_light)
{	// Check if name exists in list
	bool nametaken = true;
	std::string tmpName = a_light->m_name;
	int index = 1;
	while (nametaken == true)
	{
		nametaken = false;
		// Get list of exisiting objects
		for (auto const& obj : m_pointLights)
		{
			// if there is an object with the same name then change name of new object
			if (obj->m_name == tmpName)
			{
				nametaken = true;
				tmpName = a_light->m_name;
				tmpName += " (";
				tmpName += std::to_string(index);
				tmpName += ")";
				index++;
			}
		}
	}
	a_light->m_name = tmpName;
	m_pointLights.push_back(a_light);
}
// Draws objects and lights in the scene
void Scene::Draw()
{
	// Draw the lights first
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i]->m_direction;
		m_pointLightColors[i] = m_pointLights[i]->m_color;
		glm::vec3 normalColour = glm::normalize(m_pointLightColors[i]);
		aie::Gizmos::addSphere(m_pointLightPositions[i], 0.3, 7, 7, glm::vec4(normalColour.x, normalColour.y, normalColour.z, 1));
	}
	// Draw Instances
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}
