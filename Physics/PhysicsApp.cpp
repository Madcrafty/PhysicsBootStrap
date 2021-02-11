#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <Gizmos.h>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	
	//Increases the 2D line count to increase the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0, 0));
	// Lower value, more accurate the simulation, but less speed.
	// (To high may cause stutering)
	m_physicsScene->SetTimeStep(0.01f);

	DrawRect();

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16.0f / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DrawRect()
{
	m_physicsScene->AddActor(new Sphere(glm::vec2(20, 0), glm::vec2(-20, -180), 1, 3, glm::vec4(1,1,0,1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(-0.65, 0.75), -25));

	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -8), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(box1);
	Box* box2 = new Box(glm::vec2(0, -20), glm::vec2(16, 0), 1, 4, 8, 4, glm::vec4(0, 1, 0, 1));
	//box2->SetKinematic(true);
	m_physicsScene->AddActor(box2);
}

void PhysicsApp::SphereAndPlane()
{
	Sphere* ball1;
	ball1 = new Sphere(glm::vec2(-40, 20), glm::vec2(20, -20), 3.f, 5, glm::vec4(1, 1, 1, 1));
	m_physicsScene->AddActor(ball1);

	Sphere* ball2;
	ball2 = new Sphere(glm::vec2(40, 20), glm::vec2(-20, -20), 3.f, 5, glm::vec4(1, 1, 1, 1));
	m_physicsScene->AddActor(ball2);

	//Plane* plane = new Plane(glm::vec2(0,1), 1);
	Plane* plane = new Plane();
	m_physicsScene->AddActor(plane);
}
