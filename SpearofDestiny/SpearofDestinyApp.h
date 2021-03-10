#pragma once

#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include <glm/mat4x4.hpp>

class SpearofDestinyApp : public aie::Application {
public:

	SpearofDestinyApp();
	virtual ~SpearofDestinyApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// =====SHADER=====
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	// ================
	Mesh			   m_quadMesh;
	glm::mat4		   m_quadTransform;
	// Create a Bunny with a flat Color
	aie::OBJMesh	   m_bunnyMesh;
	glm::mat4		   m_bunnyTransform;
	
public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix);
};