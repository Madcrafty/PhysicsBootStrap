#pragma once
#include "OBJMesh.h"
#include <glm/glm.hpp>
#include <string>

class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);

	void Draw(Scene* a_scene);

	void RemakeTransform();
	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);
	glm::mat4* GetTransform() { return &m_transform; }
	glm::vec3* GetPosition() { return &m_position; }
	glm::vec3* GetRotation() { return &m_rotation; }
	glm::vec3* GetScale() { return &m_scale; }

	std::string GetName() { return m_name; };

	void SetName() { m_name = m_mesh->getFilename(); }
	void SetName(std::string a_name) { m_name = a_name; }
	// aie::OBJMesh* m_mesh;
	// aie::ShaderProgram* m_shader;
protected:
	glm::mat4 m_transform;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	std::string m_name;
};

