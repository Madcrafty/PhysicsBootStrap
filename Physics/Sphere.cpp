#include "Sphere.h"
#include <Gizmos.h>

Sphere::Sphere(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) :
	RigidBody(SPHERE, a_position, a_velocity, a_mass, 1)
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_moment = 0.5f * m_mass * a_radius * a_radius;
}

void Sphere::MakeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 100, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1 - m_colour.x, 1 - m_colour.y, 1 - m_colour.z, 1));
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