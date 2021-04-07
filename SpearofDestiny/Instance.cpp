#include "Instance.h"

#include "Scene.h"
#include "OBJMesh.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include <Texture.h>
#include <Application.h>
#include <glm/ext.hpp>

Instance::Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_transform(a_transform), m_mesh(a_mesh), m_shader(a_shader)
{
	SetName();
}

Instance::Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader)
	: m_mesh(a_mesh), m_shader(a_shader)
{
	m_transform = MakeTransform(a_position, a_eulerAngles, a_scale);
	SetName();
}

void Instance::Draw(Scene* a_scene)
{
	m_shader->bind();
	// Debug
	//m_normalMapShader.bindUniform("Ns", m_debug);
	//Bind the transform of the mesh
	auto pvm = a_scene->GetActiveCamera()->GetProjectionMatrix(a_scene->GetWindowSize().x, a_scene->GetWindowSize().y) * a_scene->GetActiveCamera()->GetViewMatrix() * m_transform; // PVM = Projection View Matrix
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("CameraPosition", a_scene->GetActiveCamera()->GetPosition());
	m_shader->bindUniform("AmbientColor", a_scene->GetAmbientLight());
	m_shader->bindUniform("LightColor", a_scene->GetLight().m_color);
	m_shader->bindUniform("LightDirection", a_scene->GetLight().m_direction);
	//m_shader->bindUniform("SpecularPower", 32);
	m_shader->bindUniform("ModelMatrix", m_transform);

	int num_lights = a_scene->GetNumLights();
	m_shader->bindUniform("numLights", num_lights);
	m_shader->bindUniform("PointLightPosition", num_lights, a_scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColor", num_lights, a_scene->GetPointLightColor());
	//m_normalMapShader.bindUniform("Ns", m_debug);

	// Bind texture to a location of your choice (0)
	//m_textureShader.bindUniform("diffuseTexture", 0);

	m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale)
{
	return glm::translate(glm::mat4(1), a_position)
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(a_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::scale(glm::mat4(1), a_scale);
}