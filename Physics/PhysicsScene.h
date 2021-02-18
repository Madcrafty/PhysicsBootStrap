#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "RigidBody.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* a_actor);
	void RemoveActor(PhysicsObject* a_actor);

	// This will call the update of each physicsObject and updates
	// internally. This will handle collision detection and resolution
	void Update(float dt);

	// Called once per frame and handles the rendering of PhysicsObjects
	// Will add to a lopp of Gizmo objects to render
	void Draw();
	void Debug();

	void SetGravity(const glm::vec2 a_gravity) { m_gravity = a_gravity; }
	glm::vec2 GetGravity() { return m_gravity; }

	void SetTimeStep(const float a_timeStep) { m_timeStep = a_timeStep; }
	float GetTimeStep() { return m_timeStep; }

	void SetClub(RigidBody* a_Club) { m_Club = a_Club; }
	RigidBody* GetClub() { return m_Club; }

	void SetBall(RigidBody* a_Ball) { m_Ball = a_Ball; }
	RigidBody* GetBall() { return m_Ball; }

	void SetHitCount(int a_hitCount) { m_hitCount = a_hitCount; }
	int GetHitCount() { return m_hitCount; }

	void SetHoleCount(int a_holeCount) { m_holeCount = a_holeCount; }
	int GetHoleCount() { return m_holeCount; }

	void SetBallInHole(bool a_state) { m_inHole = a_state; }
	bool GetBallInHole() { return m_inHole; }

	void SetScore(int a_score) { m_score = a_score; }
	int GetScore() { return m_score; }

	void SetHighScore(int a_highscore) { m_highscore = a_highscore; }
	int GetHighScore() { return m_highscore; }

	void SetSessionHighScore(int a_sessionHighscore) { m_sessionHighscore = a_sessionHighscore; }
	int GetSessionHighScore() { return m_sessionHighscore; }

	void CheckForCollision();
	static void ApplyContactForces(RigidBody* a_actor1, RigidBody* a_actor2, glm::vec2 a_collisionNormal, float a_pen);

	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);

	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*);

	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	int m_hitCount = 0;
	RigidBody* m_Club;
	RigidBody* m_Ball;
	float m_holePullMuli = 5.f;
	int m_holeCount = 0;
	int m_score = 0;
	int m_highscore = 0;
	int m_sessionHighscore = 0;
	bool m_inHole = false;

	std::vector<PhysicsObject*> m_actors;
};

