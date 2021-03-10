#include "SpearofDestinyApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

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

	return LoadShaderAndMeshLogic();
}

void SpearofDestinyApp::shutdown() {

	Gizmos::destroy();
}

void SpearofDestinyApp::update(float deltaTime) {

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

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(m_projectionMatrix, m_viewMatrix);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool SpearofDestinyApp::LoadShaderAndMeshLogic()
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

	m_quadMesh.Initialise(4, vertices, 6, indices);
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
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed\n");
	}
	m_bunnyTransform = {
		0.5f,  0,    0,	 0,
		0,	0.5f,    0,	 0,
		0,	   0, 0.5f,	 0,
		0,	   0,    0,	 1
	};
#pragma endregion

	return true;
}

void SpearofDestinyApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
#pragma region Quad
	// Bind the shader
	m_simpleShader.bind();

	//Bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; // PVM = Projection View Matrix
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	m_quadMesh.Draw();
#pragma endregion
#pragma region FlatBunny
	// Bind the shader
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0,1,0,1));

	// Draw bunny mesh
	m_bunnyMesh.draw();
#pragma endregion

}