#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) :
	RigidBody(SPHERE, a_position, a_velocity, 0, a_mass)
{
	m_radius = a_radius;
	m_mass = a_mass;
	m_colour = a_colour;
}

void Sphere::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
}

bool Sphere::CheckCollision(PhysicsObject* pOther)
{
	Sphere* pSphere = dynamic_cast<Sphere*>(pOther);
	if (pSphere != nullptr)
	{
		float distance = glm::distance(GetPosition(), pSphere->GetPosition());
		if (GetRadius() + pSphere->GetRadius() > distance)
		{
			return true;
		}
	}
	return false;
}