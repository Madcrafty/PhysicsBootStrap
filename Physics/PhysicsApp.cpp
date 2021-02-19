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

#include<iostream>
#include<fstream>

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
	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	// Lower value, more accurate the simulation, but less speed.
	// (To high may cause stutering)
	m_physicsScene->SetTimeStep(0.005f);
	
	// Load Highscore
	std::fstream hfile("./thehighscore.txt", std::ios_base::in);
	if (hfile.good())
	{
		int temp;
		hfile >> temp;
		// Set Highscore
		m_physicsScene->SetHighScore(temp);
	}

	ComboGolf();

	return true;
}

void PhysicsApp::shutdown() {

	// Save Highscore
	std::ofstream hfile;
	hfile.open("./thehighscore.txt", std::ofstream::trunc);
	hfile << m_physicsScene->GetHighScore();
	hfile.close();

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
		m_physicsScene->SetHitCount(0);
		m_physicsScene->SetHoleCount(0);
		m_physicsScene->SetScore(0);
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
	m_2dRenderer->drawText(m_font, fps, aie::Application::getWindowWidth() - 32*6, aie::Application::getWindowHeight() - 32);

	// Hit Counter
	char hc[32];
	sprintf_s(hc, 32, "Hit: %i", m_physicsScene->GetHitCount());
	m_2dRenderer->drawText(m_font, hc, 0, aie::Application::getWindowHeight() - 32);

	// Hole Counter
	char Hc[32];
	sprintf_s(Hc, 32, "Hole: %i", m_physicsScene->GetHoleCount());
	m_2dRenderer->drawText(m_font, Hc,32 * 4.5f, aie::Application::getWindowHeight() - 32);

	// Score
	char S[32];
	sprintf_s(S, 32, "Score: %i", m_physicsScene->GetScore());
	m_2dRenderer->drawText(m_font, S, 32 * 9.5f, aie::Application::getWindowHeight() - 32);

	// Highscore
	char HS[32];
	sprintf_s(HS, 32, "Highscore: %i", m_physicsScene->GetHighScore());
	m_2dRenderer->drawText(m_font, HS, aie::Application::getWindowWidth() - 32 * 12,0);

	// Session Best
	char SB[32];
	sprintf_s(SB, 32, "Session Best: %i", m_physicsScene->GetSessionHighScore());
	m_2dRenderer->drawText(m_font, SB, aie::Application::getWindowWidth() - 32 * 24, 0);

	// Ball Speed
	if (glm::length(m_physicsScene->GetBall()->GetVelocity()) > 50.f)
	{
		char Bs[32];
		sprintf_s(Bs, 32, "Ball Speed: %f", glm::length(m_physicsScene->GetBall()->GetVelocity()));
		m_2dRenderer->drawText(m_font, Bs, 32 * 18, aie::Application::getWindowHeight() - 32);
	}
	

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

//void PhysicsApp::Golf()
//{
//	// Hole
//	Sphere* Hole = new Sphere(glm::vec2(50, 0), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
//	Hole->SetKinematic(true);
//	Hole->SetTrigger(true);
//	Hole->triggerEnter = [=](PhysicsObject* other) {
//		std::cout << "Hole Entered: " << other << std::endl;
//		if (other == m_physicsScene->GetBall())
//		{
//			m_physicsScene->GetBall()->ApplyForce((Hole->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
//		}
//	};
//	Hole->triggerStay = [=](PhysicsObject* other) {
//		if (other == m_physicsScene->GetBall())
//		{
//			std::cout << "Hole Gravity " << std::endl;
//			if (glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f)
//			{
//				std::cout << "Your winner" << std::endl;
//				std::cout << "Hit Count: " << m_physicsScene->GetHitCount() << std::endl;
//			}
//			m_physicsScene->GetBall()->ApplyForce((Hole->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
//		}
//	};
//	m_physicsScene->AddActor(Hole);
//	// Ball
//	Sphere* golfBall_main = new Sphere(glm::vec2(-50, 0), glm::vec2(0, 0), 4, 4, glm::vec4(1, 0, 0, 1));
//	golfBall_main->collisionEnter = [=](PhysicsObject* other) {
//		std::cout << "Ball Entered: " << other << std::endl;
//		if (other == m_physicsScene->GetClub())
//		{
//			m_physicsScene->SetHitCount(m_physicsScene->GetHitCount() + 1);
//		}
//	};
//	m_physicsScene->SetBall(golfBall_main);
//	m_physicsScene->AddActor(golfBall_main);
//	// Club
//	Box* golfClub = new Box(glm::vec2(0, 0), glm::vec2(0, 0), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
//	m_physicsScene->SetClub(golfClub);
//	m_physicsScene->AddActor(golfClub);
//
//	// Border
//	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -50));
//	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -90));
//	m_physicsScene->AddActor(new Plane(glm::vec2(0, -1), -50));
//	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -90));
//}
void PhysicsApp::ComboGolf()
{
	// Hole1
	Sphere* Hole1 = new Sphere(glm::vec2(-30, -30), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
	Hole1->SetKinematic(true);
	Hole1->SetTrigger(true);
	Hole1->triggerEnter = [=](PhysicsObject* other) {
		std::cout << "Hole Entered: " << other << std::endl;
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->GetBall()->ApplyForce((Hole1->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole1->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	Hole1->triggerStay = [=](PhysicsObject* other) {
		if (other == m_physicsScene->GetBall())
		{
			if (glm::distance(Hole1->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f && !m_physicsScene->GetBallInHole())
			{
				std::cout << "Hole sunk!!" << std::endl;
				m_physicsScene->SetHoleCount(m_physicsScene->GetHoleCount() + 1);
				if (glm::length(m_physicsScene->GetBall()->GetVelocity()) < 50.f)
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * 2500/glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));

				}
				else 
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(),1));
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetHighScore())
				{
					m_physicsScene->SetHighScore(m_physicsScene->GetScore());
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetSessionHighScore())
				{
					m_physicsScene->SetSessionHighScore(m_physicsScene->GetScore());
				}
				m_physicsScene->SetBallInHole(true);
			}
			glm::vec2 Force = glm::vec2((Hole1->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole1->GetPosition(), m_physicsScene->GetBall()->GetPosition())));
			Force *= glm::length(m_physicsScene->GetBall()->GetVelocity() * m_physicsScene->GetTimeStep() * 100.f);
			m_physicsScene->GetBall()->ApplyForce(Force, glm::vec2(0));
			std::cout << "Hole Gravity| x: " << Force.x << " y: " << Force.y << std::endl;
		}
	};
	Hole1->triggerExit = [=](PhysicsObject* other)
	{
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->SetBallInHole(false);
		}
	};
	m_physicsScene->AddActor(Hole1);
	// Hole2
	Sphere* Hole2 = new Sphere(glm::vec2(30, -30), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
	Hole2->SetKinematic(true);
	Hole2->SetTrigger(true);
	Hole2->triggerEnter = [=](PhysicsObject* other) {
		std::cout << "Hole Entered: " << other << std::endl;
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->GetBall()->ApplyForce((Hole2->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole2->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	Hole2->triggerStay = [=](PhysicsObject* other) {
		if (other == m_physicsScene->GetBall())
		{
			if (glm::distance(Hole2->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f && !m_physicsScene->GetBallInHole())
			{
				std::cout << "Hole sunk!!" << std::endl;
				m_physicsScene->SetHoleCount(m_physicsScene->GetHoleCount() + 1);
				if (glm::length(m_physicsScene->GetBall()->GetVelocity()) < 50.f)
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * 2500 / glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));

				}
				else
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetHighScore())
				{
					m_physicsScene->SetHighScore(m_physicsScene->GetScore());
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetSessionHighScore())
				{
					m_physicsScene->SetSessionHighScore(m_physicsScene->GetScore());
				}
				m_physicsScene->SetBallInHole(true);
			}
			glm::vec2 Force = glm::vec2((Hole2->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole2->GetPosition(), m_physicsScene->GetBall()->GetPosition())));
			Force *= glm::length(m_physicsScene->GetBall()->GetVelocity() * m_physicsScene->GetTimeStep() * 100.f);
			m_physicsScene->GetBall()->ApplyForce(Force, glm::vec2(0));
			std::cout << "Hole Gravity| x: " << Force.x << " y: " << Force.y << std::endl;
		}
	};
	Hole2->triggerExit = [=](PhysicsObject* other)
	{
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->SetBallInHole(false);
		}
	};
	m_physicsScene->AddActor(Hole2);
	// Hole3
	Sphere* Hole3 = new Sphere(glm::vec2(30, 30), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
	Hole3->SetKinematic(true);
	Hole3->SetTrigger(true);
	Hole3->triggerEnter = [=](PhysicsObject* other) {
		std::cout << "Hole Entered: " << other << std::endl;
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->GetBall()->ApplyForce((Hole3->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole3->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	Hole3->triggerStay = [=](PhysicsObject* other) {
		if (other == m_physicsScene->GetBall())
		{
			if (glm::distance(Hole3->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f && !m_physicsScene->GetBallInHole())
			{
				std::cout << "Hole sunk!!" << std::endl;
				m_physicsScene->SetHoleCount(m_physicsScene->GetHoleCount() + 1);
				if (glm::length(m_physicsScene->GetBall()->GetVelocity()) < 50.f)
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * 2500 / glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));

				}
				else
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetHighScore())
				{
					m_physicsScene->SetHighScore(m_physicsScene->GetScore());
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetSessionHighScore())
				{
					m_physicsScene->SetSessionHighScore(m_physicsScene->GetScore());
				}
				m_physicsScene->SetBallInHole(true);
			}
			glm::vec2 Force = glm::vec2((Hole3->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole3->GetPosition(), m_physicsScene->GetBall()->GetPosition())));
			Force *= glm::length(m_physicsScene->GetBall()->GetVelocity() * m_physicsScene->GetTimeStep() * 100.f);
			m_physicsScene->GetBall()->ApplyForce(Force, glm::vec2(0));
			std::cout << "Hole Gravity| x: " << Force.x << " y: " << Force.y << std::endl;
		}
	};
	Hole3->triggerExit = [=](PhysicsObject* other)
	{
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->SetBallInHole(false);
		}
	};
	m_physicsScene->AddActor(Hole3);
	// Hole4
	Sphere* Hole4 = new Sphere(glm::vec2(-30, 30), glm::vec2(0, 0), 4, 4, glm::vec4(0.3f, 0.3f, 0.3f, 1));
	Hole4->SetKinematic(true);
	Hole4->SetTrigger(true);
	Hole4->triggerEnter = [=](PhysicsObject* other) {
		std::cout << "Hole Entered: " << other << std::endl;
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->GetBall()->ApplyForce((Hole4->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole4->GetPosition(), m_physicsScene->GetBall()->GetPosition())), glm::vec2(0));
		}
	};
	Hole4->triggerStay = [=](PhysicsObject* other) {
		if (other == m_physicsScene->GetBall())
		{
			if (glm::distance(Hole4->GetPosition(), m_physicsScene->GetBall()->GetPosition()) < 4.f && !m_physicsScene->GetBallInHole())
			{
				std::cout << "Hole sunk!!" << std::endl;
				m_physicsScene->SetHoleCount(m_physicsScene->GetHoleCount() + 1);
				if (glm::length(m_physicsScene->GetBall()->GetVelocity()) < 50.f)
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * 2500 / glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));

				}
				else
				{
					m_physicsScene->SetScore(m_physicsScene->GetScore() + 10 * glm::length(m_physicsScene->GetBall()->GetVelocity()) * glm::max(m_physicsScene->GetHitCount(), 1));
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetHighScore())
				{
					m_physicsScene->SetHighScore(m_physicsScene->GetScore());
				}
				if (m_physicsScene->GetScore() > m_physicsScene->GetSessionHighScore())
				{
					m_physicsScene->SetSessionHighScore(m_physicsScene->GetScore());
				}
				m_physicsScene->SetBallInHole(true);
			}
			glm::vec2 Force = glm::vec2((Hole4->GetPosition() - m_physicsScene->GetBall()->GetPosition()) * (1.f / glm::distance(Hole4->GetPosition(), m_physicsScene->GetBall()->GetPosition())));
			Force *= glm::length(m_physicsScene->GetBall()->GetVelocity() * m_physicsScene->GetTimeStep() * 100.f);
			m_physicsScene->GetBall()->ApplyForce(Force, glm::vec2(0));
			std::cout << "Hole Gravity| x: " << Force.x << " y: " << Force.y << std::endl;
		}
	};
	Hole4->triggerExit = [=](PhysicsObject* other)
	{
		if (other == m_physicsScene->GetBall())
		{
			m_physicsScene->SetBallInHole(false);
		}
	};
	m_physicsScene->AddActor(Hole4);
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
