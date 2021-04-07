#pragma once
#include <glm/glm.hpp>
#include <string>
class Camera
{
public:
	Camera();
	Camera(std::string a_name, bool a_dynamic);
	Camera(glm::vec3 a_position, float a_theta, float a_phi, bool a_dynamic);
	Camera(glm::vec3 a_position, float a_theta, float a_phi, bool a_dynamic, std::string a_name);
	~Camera() {};

	void Update(float a_deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	std::string GetName() { return m_name; }
	void SetName() { m_name = "Light"; }
	void SetName(std::string a_name) { m_name = a_name; }
	bool IsDynamic() { return m_dynamic; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);
private:
	bool m_dynamic;
	std::string m_name;
	float m_theta; // In degrees
	float m_phi; // In degrees
	glm::vec3 m_position;
	
	float m_lastMouseX, m_lastMouseY;
public:
	float m_speed = 1;
	float m_sensitivity = 1;
};

