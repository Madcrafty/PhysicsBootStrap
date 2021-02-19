#pragma once
#include "PhysicsObject.h"
#include <list>
#include <functional>
#include <iostream>
class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_rotation);
	~RigidBody() {};

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() {};
	void ApplyForce(glm::vec2 a_force, glm::vec2 a_pos);
	
	void ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, float a_pen, glm::vec2* a_collisionNormal = nullptr);

	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetRoation() { return m_rotation; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment;; }
	float GetLinearDrag() { return m_linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }

	glm::vec2 SetPosition(glm::vec2 a_position) { return m_position = a_position; }
	glm::vec2 SetVelocity(glm::vec2 a_velocity) { return m_velocity = a_velocity; }
	float SetRotation(float a_rotation) { return m_rotation = a_rotation; }
	float SetAngularVelocity(float a_angularVelocity) { return m_angularVelocity = a_angularVelocity; }

	glm::vec2 ToWorld(glm::vec2 a_localPos);

	std::function<void(PhysicsObject*)> m_collisionCallback;

	void TriggerEntered(PhysicsObject* a_otherActor);
	void TriggerStaying(PhysicsObject* a_otherActor);
	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerStay;
	std::function<void(PhysicsObject*)> triggerExit;

	void CollisionEntered(PhysicsObject* a_otherActor);
	std::function<void(PhysicsObject*)> collisionEnter;
	std::function<void(PhysicsObject*)> collisionExit;

	bool IsTrigger() { return m_isTrigger; }
	bool SetTrigger(bool a_state) { return m_isTrigger = a_state; }

protected:
	bool m_isTrigger;
	std::list<PhysicsObject*> m_objectInside;
	std::list<PhysicsObject*> m_objectInsideThisFrame;

	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_mass;
	float m_rotation;

	float m_angularVelocity;
	float m_moment;

	float m_linearDrag;
	float m_angularDrag;

	// These will store the local x and y axes of the box based on its angle of rotation
	glm::vec2 m_localX ;
	glm::vec2 m_localY;
};

