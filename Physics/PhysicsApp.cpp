#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
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

	Golf();

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

	// Mouse pos tracking
	int xScreen, yScreen;
	input->getMouseXY(&xScreen, &yScreen);
	glm::vec2 mouseWorldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
	m_mouseVelocity = (mouseWorldPos - m_previousMousePos) / deltaTime;

	if (input->isMouseButtonDown(0))
	{
		aie::Gizmos::add2DCircle(mouseWorldPos, 5, 32, glm::vec4(0.3));
		m_physicsScene->GetClub()->SetPosition(mouseWorldPos);
		m_physicsScene->GetClub()->SetVelocity(m_mouseVelocity);
	}
	if (!input->isMouseButtonDown(0) && m_previousMouseState1)
	{
		m_physicsScene->GetClub()->SetPosition(mouseWorldPos);
		m_physicsScene->GetClub()->SetVelocity(m_mouseVelocity);
	}
	// Updating previous mousePos;
	m_previousMousePos = mouseWorldPos;
	m_previousMouseState1 = input->isMouseButtonDown(0);
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	//X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!

	// FPS: Counter
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// Hit Counter
	char hc[32];
	sprintf_s(hc, 32, "Hit: %i", m_physicsScene->GetHitCount());
	m_2dRenderer->drawText(m_font, hc, 1280 - 32*32, 720 - 32);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the the center of the screen 0,0
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// Scale this according to the extents
	worldPos.x *= 2.f * m_extents / getWindowWidth();
	worldPos.y *= 2.f * m_extents / getWindowHeight() / m_aspectRatio;

	return worldPos;
}

void PhysicsApp::Golf()
{
	// Hole
	Sphere* Hole = new Sphere(glm::vec2(50, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
	Hole->SetKinematic(true);
	Hole->SetTrigger(true);
	Hole->triggerEnter = [=](PhysicsObject* other) {
		std::cout << "Hole Entered: " << other << std::endl;
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->GetBall()->ApplyForce((Hole->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	Hole->triggerStay = [=](PhysicsObject* other) {
		if (other == m_physicsScene->GetBall())
		{
			std::cout << "Hole Gravity " << std::endl;
			if (glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f)
			{
				std::cout << "Your winner" << std::endl;
				std::cout << "Hit Count: " << m_physicsScene->GetHitCount() << std::endl;
			}
			m_physicsScene->GetBall()->ApplyForce((Hole->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	m_physicsScene->AddActor(Hole);
	// Ball
	Sphere* golfBall_main = new Sphere(glm::vec2(-50, 0), glm::vec2(0, 0), 4, 4, glm::vec4(1, 0, 0, 1));
	golfBall_main->collisionEnter = [=](PhysicsObject* other) {
		std::cout << "Ball Entered: " << other << std::endl;
		if (other == m_physicsScene->GetClub())
		{
			m_physicsScene->SetHitCount(m_physicsScene->GetHitCount() + 1);
		}
	};
	m_physicsScene->SetBall(golfBall_main);
	m_physicsScene->AddActor(golfBall_main);
	// Club
	Box* golfClub = new Box(glm::vec2(0, 0), glm::vec2(0, 0), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
	m_physicsScene->SetClub(golfClub);
	m_physicsScene->AddActor(golfClub);
	
	// Border
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -90));
	m_physicsScene->AddActor(new Plane(glm::vec2(0, -1), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -90));
}

void PhysicsApp::TriggerTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(0, 0), glm::vec2(20, 0), 4, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(10, -0), glm::vec2(0), 4, 4, glm::vec4(0, 1, 0, 1));
	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0,1),-30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1,0),-50));
	m_physicsScene->AddActor(new Plane(glm::vec2(0,-1),-50));
	m_physicsScene->AddActor(new Box(glm::vec2(20,10), glm::vec2(3,0), 0.5, 4 ,8, 4));
	m_physicsScene->AddActor(new Box(glm::vec2(-40,10), glm::vec2(3,0), 0.5, 4 ,8, 4));

	ball2->triggerEnter = [=](PhysicsObject* other) {std::cout << "Entered: " << other << std::endl;  };
	ball2->triggerExit = [=](PhysicsObject* other) {std::cout << "Exited: " << other << std::endl;  };
}

void PhysicsApp::DrawRect()
{
	Sphere* ball = new Sphere(glm::vec2(20, 0), glm::vec2(-20, -180), 1, 3, glm::vec4(1, 1, 0, 1));
	m_physicsScene->AddActor(ball);
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

void PhysicsApp::SpringTest(int a_amount)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < a_amount; i++)
	{
		// This will need to spawn the sphere at the bottom of the previous one, to
		// make a pendulum that is effected by gravity
		Sphere* sphere = new Sphere(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(1, 1, 1, 1));
		if (i == 0)
		{
			sphere->SetKinematic(true);	
		}
		m_physicsScene->AddActor(sphere);
		if (prev)
		{
			m_physicsScene->AddActor(new Spring(sphere, prev, 10, 500));
		}
		prev = sphere;
	}
	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2);
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);
}
