#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "OBJMesh.h"
#include <glm/mat4x4.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtx/transform.hpp>
#include "Scene.h"
#include "Instance.h"

class SpearofDestinyApp : public aie::Application {
public:

	SpearofDestinyApp();
	virtual ~SpearofDestinyApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Texture		m_gridTexture;

	// =====SHADER=====
	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_bunnyShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_textureShader;
	aie::ShaderProgram	m_normalMapShader;
	// =====OBJECTS=====
	// Quad Base
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;
	float				m_quadSpecPower;
	// Create a Bunny with a flat Color
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;
	float				m_bunnySpecPower;
	// Dragoon
	aie::OBJMesh		m_dragoonMesh;
	glm::mat4			m_dragoonTransform;
	float				m_dragoonSpecPower;
	// SoulSpear
	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;
	float				m_spearSpecPower;
	// ShotGun
	aie::OBJMesh		m_shotgunMesh;
	glm::mat4			m_shotgunTransform;
	float				m_shotgunSpecPower;

	Scene* m_scene;
	Instance* m_selectedObject = nullptr;
	Light* m_selectedLight = nullptr;
	Camera* m_selectedCamera = nullptr;

	// Quaturions
	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	void DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix);
	void IMGUI_Logic();
};