#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include "PhysicsScene.h"

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	const float m_aspectRatio = 16.0f / 9.0f;
	const float m_extents = 100;

	glm::vec2 m_previousMousePos = glm::vec2(0,0);
	bool m_previousMouseState1 = false; // false = not pressed, true = pressed
	glm::vec2 m_mouseVelocity = glm::vec2(0,0);

public: // Scenes
	void ComboGolf();
	// void Golf();
	// Test Scenes
	void TriggerTest();
	void DrawRect();
	void SphereAndPlane();
	void SpringTest(int a_amount);
};