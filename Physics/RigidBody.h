#pragma once
#include "PhysicsObject.h"
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

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_mass;
	float m_rotation;

	float m_angularVelocity;
	float m_moment;

	float m_linearDrag;
	float m_angularDrag;
};

