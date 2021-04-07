#pragma once
#include <string>
#include <list>
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light 
{
	Light() {
		m_direction = glm::vec3(0);
		m_color = glm::vec3(1);
		m_name = "Light";
	}
	Light(glm::vec3 a_pos, glm::vec3 a_color, float a_intensity)
	{
		m_direction = a_pos;
		m_color = a_color * a_intensity;
		m_name = "Light";
	}
	Light(glm::vec3 a_pos, glm::vec3 a_color, float a_intensity, std::string a_name)
	{
		m_direction = a_pos;
		m_color = a_color * a_intensity;
		m_name = a_name;
	}
	glm::vec3 m_direction;
	glm::vec3 m_color;
	std::string m_name;
};

class Scene
{
public:
	Scene(glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight);
	Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight);
	~Scene();

	void AddInstance(Instance* a_instance);
	void AddCamera(Camera* a_camera);
	void AddLight(Light* a_light);
	void Draw();

	Camera* GetActiveCamera() { return m_activeCamera; }
	void SetActiveCamera(Camera* a_camera) { m_activeCamera = a_camera; }
	std::list<Camera*> GetCameras() { return m_cameras; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	Light& GetLight() { return m_light; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
	std::list<Instance*>& GetInstances() { return m_instances; }

	int GetNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColor() { return &m_pointLightColors[0]; }

	std::vector<Light*>& GetPointLights() { return m_pointLights; }

protected:
	Camera* m_activeCamera;
	std::list<Camera*> m_cameras;
	glm::vec2 m_windowSize;
	Light m_light;
	Light m_sunlight;
	std::vector<Light*> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
};

