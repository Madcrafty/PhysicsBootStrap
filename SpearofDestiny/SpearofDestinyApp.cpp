#include "SpearofDestinyApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include<imgui.h>



using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

glm::vec3 mercury = glm::vec3(1.5, 0, 0);

SpearofDestinyApp::SpearofDestinyApp() {

}

SpearofDestinyApp::~SpearofDestinyApp() {

}

bool SpearofDestinyApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth()/(float)getWindowHeight(), 0.1f, 1000.0f);

	Light light;
	light.m_color = { 1,1,1 };

	return LoadShaderAndMeshLogic(light);
}

void SpearofDestinyApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void SpearofDestinyApp::update(float deltaTime) {
	IMGUI_Logic();
	m_camera.Update(deltaTime);
	float time = getTime();

	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}
void drawplanets()
{
	// 1 unit = suns radius (696,340km)

// Sun

	Gizmos::addSphere(glm::vec3(0, 0, 0), 1, 12, 12, glm::vec4(1, 1, 0.3, 1));

	// Mercury
	Gizmos::addSphere(mercury, 0.1, 12, 12, glm::vec4(0.7, 0.7, 0.7, 1));

	// Venus
	Gizmos::addSphere(glm::vec3(2.3, 0, 0), 0.2, 12, 12, glm::vec4(1, 0.7, 0.3, 1));

	// Earth
	Gizmos::addSphere(glm::vec3(3, 0, 0), 0.2, 12, 12, glm::vec4(0.1, 0.6, 1, 1));

	// Mars
	Gizmos::addSphere(glm::vec3(3.8, 0, 0), 0.13, 12, 12, glm::vec4(1, 0.4, 0.2, 1));

	// Jupiter
	Gizmos::addSphere(glm::vec3(5, 0, 0), 0.5, 12, 12, glm::vec4(1, 0.7, 0.5, 1));

	// Saturn

	Gizmos::addSphere(glm::vec3(6.5, 0, 0), 0.4, 12, 12, glm::vec4(1, 0.8, 0.4, 1));
	Gizmos::addArcRing(glm::vec3(6.5, 0, 0), 0, 0.5, 0.6, 4, 20, glm::vec4(1, 1, 1, 1));

	// Neptune
	Gizmos::addSphere(glm::vec3(7.7, 0, 0), 0.3, 12, 12, glm::vec4(0.1, 0.1, 1, 1));
	Gizmos::addArcRing(glm::vec3(7.7, 0, 0), 0, 0.4, 0.5, 4, 20, glm::vec4(1, 1, 1, 1));

	// Uranus
	Gizmos::addSphere(glm::vec3(8.7, 0, 0), 0.3, 12, 12, glm::vec4(0.1, 1, 0.7, 1));
}

void SpearofDestinyApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_camera.GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	//DrawShaderAndMeshes(projectionMatrix, viewMatrix);
	m_scene->Draw();

	Gizmos::draw(projectionMatrix * viewMatrix);
}

bool SpearofDestinyApp::LoadShaderAndMeshLogic(Light a_light)
{
#pragma region Quad
	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}
	//m_quadMesh.InitialiseQuad();
	//Mesh::Vertex vertices[6];
	//vertices[0].position = { -0.5f, 0.f, 0.5f, 1.f };
	//vertices[1].position = { 0.5f, 0.f, 0.5f, 1.f };
	//vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };

	//vertices[3].position = { -0.5f, 0.f, -0.5f, 1.f };
	//vertices[4].position = { 0.5f, 0.f, 0.5f, 1.f };
	//vertices[5].position = { 0.5f, 0.f, -0.5f, 1.f };

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.f };
	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.f };
	vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };
	vertices[3].position = { 0.5f, 0.f, -0.5f, 1.f };

	unsigned int indices[6] = { 0,1,2,2,1,3 };

	m_quadMesh.InitialiseQuad();
	//m_quadMesh.Initialise(4, vertices, 6, indices);
	// We will make the quad 10 units by 
	m_quadTransform = {
		10,	0,	0,	0,
		0,	10,	0,	0,
		0,	0,	10,	0,
		0,	0,	0,	1
	};
#pragma endregion
#pragma region FlatBunny
	// Load the vertex shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_bunnyShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region Phong
	// Load the vertex shader from a file
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");

	// Load the fragment shader from a file
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (!m_phongShader.link())
	{
		printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region TextureShader
	// Load the vertex shader from a file
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/texture.vert");

	// Load the fragment shader from a file
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/texture.frag");
	if (!m_textureShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_textureShader.getLastError());
		return false;
	}
#pragma endregion
#pragma region NormalMap
	// Load the vertex shader from a file
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");

	// Load the fragment shader from a file
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	
	if (!m_normalMapShader.link())
	{
		printf("Normal Shader had an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion
	// Load meshes
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed\n");
		return false;
	}
	m_bunnyTransform = {
		0.5f,  0,    0,	 0,
		0,	0.5f,    0,	 0,
		0,	   0, 0.5f,	 0,
		0,	   0,    0,	 1
	};
	if (m_dragoonMesh.load("./stanford/Dragon.obj") == false)
	{
		printf("Dragon Mesh Failed\n");
		return false;
	}
	m_dragoonTransform = {
		0.6f,	0,		0,		0,
		0,		0.6f,	0,		0,
		0,		0,		0.6f,	0,
		0,		0,		0,		 1
	};	
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Mesh Failed\n");
		return false;
	}
	m_spearTransform = {
		1.0f,	0,		0,		0,
		0,		1.0f,	0,		0,
		0,		0,		1.0f,	0,
		0,		0,		0,		 1
	};
	if (m_shotgunMesh.load("./shotgun/new_shotgun.obj", true, true) == false)
	{
		printf("Shotgun Mesh Failed\n");
		return false;
	}
	m_shotgunTransform = {
		1.0f,	0,		0,		0,
		0,		1.0f,	0,		0,
		0,		0,		1.0f,	0,
		0,		0,		0,		 1
	};
	//m_dragoonTransform = {
	//	0.6f,	0,		0,		0,
	//	0,		0.6f,	0,		0,
	//	0,		0,		0.6f,	0,
	//	0,		0,		0,		 1
	//};
	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));
	for (int i = 0; i < 10; i++)
	{
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, i * 30, 0), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
	}
	// red light - left
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	// green light - right
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));
	return true;
}



void SpearofDestinyApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.f, 1.f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.f);
	//ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.f, 1.f);
	//ImGui::DragFloat3("Sunlight Colour", &m_light.color[0], 0.1f, 0.0f, 2.f);
	ImGui::DragFloat("Specular Term Power", &m_debug, 0.05f);
	ImGui::End();

	ImGui::Begin("Heirarchy");
	ImGui::ListBoxHeader("Objects:");
	//for (auto const& obj : m_scene->GetInstances())
	//{
	//	if (ImGui::Selectable(obj->GetName(), (obj == m_selectedObject) ? true : false))
	//	{
	//		m_selectedObject = obj;
	//	}
	//}
	for (auto i = m_scene->GetInstances().begin(); i != m_scene->GetInstances().end(); i++)
	{
		if (ImGui::Selectable((*i)->GetName(), (*i == m_selectedObject)? true : false))
		{
			m_selectedObject = *i;
		}
	}
	ImGui::ListBoxFooter();
	ImGui::End();
	if (m_selectedObject != nullptr)
	{
		ImGui::Begin("Inspector");
		glm::mat4* selected = m_selectedObject->GetTransform();
		float* row1[4] = { &selected[0][0].x, &selected[0][0].y, &selected[0][0].z, &selected[0][0].w };
		float* row2[4] = { &selected[0][1].x, &selected[0][1].y, &selected[0][1].z, &selected[0][1].w };
		float* row3[4] = { &selected[0][2].x, &selected[0][2].y, &selected[0][2].z, &selected[0][2].w };
		float* row4[4] = { &selected[0][3].x, &selected[0][3].y, &selected[0][3].z, &selected[0][3].w };

		ImGui::DragFloat4("Transform Matrix1", *row1, 0.1f);
		ImGui::DragFloat4("Transform Matrix2", *row2, 0.1f);
		ImGui::DragFloat4("Transform Matrix3", *row3, 0.1f);
		ImGui::DragFloat4("Transform Matrix4", *row4, 0.1f);

		//ImGui::DragFloat("specularPower", &m_shotgunSpecPower, 0.05f);
		ImGui::End();
	}


	ImGui::Begin("Camera Controle");
	ImGui::DragFloat("Camera Speed", &m_camera.m_speed, 0.05f);
	ImGui::DragFloat("Camera Sensitivity", &m_camera.m_sensitivity, 0.05f);
	ImGui::End();
}

//void SpearofDestinyApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
//{
//	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
//#pragma region Quad
//	// Bind the shader
//	m_textureShader.bind();
//
//	//Bind the transform of the mesh
//	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; // PVM = Projection View Matrix
//	m_textureShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind texture to a location of your choice (0)
//	m_textureShader.bindUniform("diffuseTexture", 0);
//
//	m_gridTexture.bind(0);
//
//	m_quadMesh.Draw();
//#pragma endregion
//#pragma region FlatBunny
//	// Bind the shader
//	m_bunnyShader.bind();
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind texture to a location of your choice (0)
//	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0,1,0,1));
//
//	// Draw bunny mesh
//	//m_bunnyMesh.draw();
//#pragma endregion
//#pragma region Phong
//	// Bind the shader
//	m_phongShader.bind();
//
//	// Bind camera position
//	m_phongShader.bindUniform("CameraPosition", m_camera.GetPosition());
//
//	// Bind the light
//	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
//	m_phongShader.bindUniform("LightColor", m_light.color);
//	m_phongShader.bindUniform("LightDirection", m_light.direction);
//	
//
//	// Debug
//	m_phongShader.bindUniform("Ns", m_debug);
//	// Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
//
//	// Draw bunny mesh
//	m_bunnyMesh.draw();
//
//	// Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_dragoonTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_dragoonTransform);
//
//	// Draw dragoon mesh
//	m_dragoonMesh.draw();
//#pragma endregion
//	m_normalMapShader.bind();
//	// Debug
//	m_normalMapShader.bindUniform("Ns", m_debug);
//	//Bind the transform of the mesh
//	pvm = a_projectionMatrix * a_viewMatrix * m_spearTransform; // PVM = Projection View Matrix
//	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//	m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	m_normalMapShader.bindUniform("LightColor", m_light.color);
//	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//	m_normalMapShader.bindUniform("ModelMatrix", m_spearTransform);
//	//m_normalMapShader.bindUniform("Ns", m_debug);
//
//	// Bind texture to a location of your choice (0)
//	//m_textureShader.bindUniform("diffuseTexture", 0);
//
//	m_spearMesh.draw();
//
//	pvm = a_projectionMatrix * a_viewMatrix * m_shotgunTransform; // PVM = Projection View Matrix
//	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//	m_normalMapShader.bindUniform("ModelMatrix", m_shotgunTransform);
//
//	m_shotgunMesh.draw();
//}