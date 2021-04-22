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

	m_positions[0] = glm::vec3(10, 5, 10);
	m_positions[1] = glm::vec3(-10, 0, -10);
	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));

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
	m_scene->GetActiveCamera()->Update(deltaTime);
	float time = getTime();

	//m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
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

	// Quaturnion stuff
	// use time to animate a alue between [0, 1]
	float s = glm::cos(getTime()) * 0.5f + 0.5f;
	
	// standard linear interpolation
	glm::vec3 p = (1.0f -s) * m_positions[0] + s* m_positions[1];
	
	// quaternion slerp
	glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	
	// build a matrix
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	
	// draw a transform and box
	Gizmos::addTransform(m);
	Gizmos::addAABBFilled(glm::vec3(0), glm::vec3(.5f), glm::vec4(1,0,0,1), &m);

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
	
	glm::mat4 projectionMatrix = m_scene->GetActiveCamera()->GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_scene->GetActiveCamera()->GetViewMatrix();

	// update perspective based on screen size
	//m_scene->GetCamera()->GetProjectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

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
#pragma region LoadMeshes
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
#pragma endregion
	m_scene = new Scene(glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));
	for (int i = 0; i < 2; i++)
	{
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, i * 30, 0), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
	}
	m_scene->AddInstance(new Instance(glm::vec3(-5, 1, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &m_shotgunMesh, &m_normalMapShader));
	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &m_dragoonMesh, &m_phongShader));
	// red light - left
	m_scene->AddLight(new Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	//m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	// green light - right
	m_scene->AddLight(new Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));
	//m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));
	m_scene->AddCamera(new Camera(glm::vec3(20, 0, 0),180,0, false));
	m_scene->AddCamera(new Camera(glm::vec3(0, 20, 0),0,-90, false));
	m_scene->AddCamera(new Camera(glm::vec3(0, 0, 20),-90,0, false));
	return true;
}


void SpearofDestinyApp::IMGUI_Logic()
{
	ImGui::Begin("the only moving light");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.f, 1.f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.f);
	//ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.f, 1.f);
	//ImGui::DragFloat3("Sunlight Colour", &m_light.color[0], 0.1f, 0.0f, 2.f);
	//ImGui::DragFloat("Specular Term Power", &m_debug, 0.05f);
	ImGui::End();

	ImGui::Begin("Heirarchy");
	ImGui::ListBoxHeader("Objects:");
	for (auto const& obj : m_scene->GetInstances())
	{
		if (ImGui::Selectable(obj->GetName().c_str(), (obj == m_selectedObject) ? true : false))
		{
			m_selectedObject = obj;
		}
	}
	//for (auto i = m_scene->GetInstances().begin(); i != m_scene->GetInstances().end(); i++)
	//{
	//	if (ImGui::Selectable((*i)->GetName(), (*i == m_selectedObject)? true : false))
	//	{
	//		m_selectedObject = *i;
	//	}
	//}
	ImGui::ListBoxFooter();
	ImGui::ListBoxHeader("Lights:");
	for (auto const& obj : m_scene->GetPointLights())
	{
		if (ImGui::Selectable(obj->m_name.c_str(), (obj == m_selectedLight) ? true : false))
		{
			m_selectedLight = obj;
		}
	}
	ImGui::ListBoxFooter();
	ImGui::ListBoxHeader("Cameras:");
	for (auto const& obj : m_scene->GetCameras())
	{
		if (ImGui::Selectable(obj->GetName().c_str(), (obj == m_selectedCamera) ? true : false))
		{
			m_selectedCamera = obj;
			m_scene->SetActiveCamera(m_selectedCamera);
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

		//float* position[3] = { &selected[0][3].x, &selected[0][3].y, &selected[0][3].z };
		//float* position = &selected[0][3].x;
		float f_position[3] = { m_selectedObject->GetPosition()->x, m_selectedObject->GetPosition()->y, m_selectedObject->GetPosition()->z };
		//float rotation[3] = glm::rotation(1, 1, 1);
		float f_rotation[3] = { m_selectedObject->GetRotation()->x, m_selectedObject->GetRotation()->y, m_selectedObject->GetRotation()->z };
		// Scale values are not adjacent so it isn't correctly read with ImGui.
		// Resorted to creating a tmp value to store the scale
		//float scale[3] = { selected[0][0].x, selected[0][1].y, selected[0][2].z };
		float f_scale[3] = { m_selectedObject->GetScale()->x, m_selectedObject->GetScale()->y, m_selectedObject->GetScale()->z };

		// Quaturnions
		glm::quat quatRotation = glm::toQuat(*selected);
		//glm::quat targetRotation = curRotation;
		//float tmpTarget[4]{ targetRotation.x, targetRotation.y, targetRotation.z, targetRotation.w };

		//float tmpTarget[4]{ curRotation.x, curRotation.y, curRotation.z, curRotation.w };

		ImGui::Text("Transform");
		ImGui::DragFloat4(" ", *row1, 0.1f);
		ImGui::DragFloat4("  ", *row2, 0.1f);
		ImGui::DragFloat4("   ", *row3, 0.1f);
		ImGui::DragFloat4("    ", *row4, 0.1f);

		ImGui::Text("Tools");
		ImGui::DragFloat3("Position", f_position, 0.1f);
		ImGui::DragFloat3("Rotation", f_rotation, 0.1f);
		ImGui::DragFloat3("Scale", f_scale, 0.1f);

		*m_selectedObject->GetPosition() = glm::vec3(f_position[0], f_position[1], f_position[2]);
		*m_selectedObject->GetRotation() = glm::vec3(f_rotation[0], f_rotation[1], f_rotation[2]);
		*m_selectedObject->GetScale() = glm::vec3(f_scale[0], f_scale[1], f_scale[2]);
		// Updating transform scale
		//selected[0][0].x = scale[0];
		//selected[0][1].y = scale[1];
		//selected[0][2].z = scale[2];
		

		//curRotation.x = tmpTarget[0];
		//curRotation.y = tmpTarget[1];
		//curRotation.z = tmpTarget[2];
		//curRotation.w = tmpTarget[3];

		//glm::vec3 tmpPosition = glm::vec3(position[0], position[1], position[2]);
		//glm::vec3 tmpScale = glm::vec3(scale[0], scale[1], scale[2]);
		//*selected = glm::translate(tmpPosition) * glm::toMat4(glm::normalize(curRotation));
		m_selectedObject->RemakeTransform();

		//ImGui::DragFloat("specularPower", &m_shotgunSpecPower, 0.05f);
		ImGui::End();
	}
	if (m_selectedLight != nullptr)
	{
		ImGui::Begin("Active light");
		ImGui::DragFloat3("Position", &m_selectedLight->m_direction[0], 0.1f);
		ImGui::DragFloat3("Colour", &m_selectedLight->m_color[0], 0.1f);
		ImGui::End();
	}
	if (m_scene->GetActiveCamera()->IsDynamic())
	{
		ImGui::Begin("Camera Controle");
		ImGui::DragFloat("Camera Speed", &m_scene->GetActiveCamera()->m_speed, 0.05f);
		ImGui::DragFloat("Camera Sensitivity", &m_scene->GetActiveCamera()->m_sensitivity, 0.05f);
		ImGui::End();
	}
}