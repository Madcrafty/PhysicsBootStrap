#include "Instance.h"

#include "Scene.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include <Texture.h>
#include <Application.h>
#include <glm/ext.hpp>

// Constructors
Instance::Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_transform(a_transform), m_mesh(a_mesh), m_shader(a_shader)
{
	SetName();
}

Instance::Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_mesh(a_mesh), m_shader(a_shader)
{
	m_position = a_position;
	m_rotation = a_eulerAngles;
	m_scale = a_scale;
	m_transform = MakeTransform(a_position, a_eulerAngles, a_scale);
	SetName();
}

// Draw's object to active projection matrix
void Instance::Draw(Scene* a_scene)
{
	m_shader->bind();
	//Bind the transform of the mesh
	auto pvm = a_scene->GetActiveCamera()->GetProjectionMatrix(a_scene->GetWindowSize().x, a_scene->GetWindowSize().y) * a_scene->GetActiveCamera()->GetViewMatrix() * m_transform; // PVM = Projection View Matrix
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("CameraPosition", a_scene->GetActiveCamera()->GetPosition());
	m_shader->bindUniform("AmbientColor", a_scene->GetAmbientLight());
	m_shader->bindUniform("LightColor", a_scene->GetDirectionalLight().m_color);
	m_shader->bindUniform("LightDirection", a_scene->GetDirectionalLight().m_direction);
	m_shader->bindUniform("ModelMatrix", m_transform);

	int num_lights = a_scene->GetNumLights();
	m_shader->bindUniform("numLights", num_lights);
	m_shader->bindUniform("PointLightPosition", num_lights, a_scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColor", num_lights, a_scene->GetPointLightColor());

	m_mesh->draw();
}

// Creates the transform of the object
glm::mat4 Instance::MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale)
{
	return glm::translate(glm::mat4(1), a_position)
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), a_scale);
}
void Instance::RemakeTransform()
{
	m_transform = glm::translate(glm::mat4(1), m_position)
		* glm::rotate(glm::mat4(1), glm::radians(m_rotation.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(m_rotation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(m_rotation.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), m_scale);
}