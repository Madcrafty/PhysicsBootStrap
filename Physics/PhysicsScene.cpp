#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <list>
#include <iostream>
#include <vector>


typedef bool (*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
		delete pActor;
}

void PhysicsScene::AddActor(PhysicsObject* a_actor)
{
	m_actors.push_back(a_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* a_actor)
{
	auto it = std::find(m_actors.begin(), m_actors.end(), a_actor);
	if (it != m_actors.end())
		m_actors.erase(it);
}

void PhysicsScene::Update(float dt)
{
	static std::list<PhysicsObject*> dirty;
	static float accumulatedTime = 0.f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
		CheckForCollision();
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::Debug()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->Debug();
		count++;
	}
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* objOuter = m_actors[outer];
			PhysicsObject* objInner = m_actors[inner];
			int shapeID_out = objOuter->GetShapeID();
			int shapeID_in = objInner->GetShapeID();

			// This will check to ensure we do not include joints
			if (shapeID_in >= 0 && shapeID_out >= 0)
			{
				// Uses our function pointers (fn)
				int functionIndex = (shapeID_out * SHAPE_COUNT) + shapeID_in;
				fn collisionFunctionPtr = collisionFunctionArray[functionIndex];
				if (collisionFunctionPtr != nullptr)
				{
					// Check if the collision occurs
					collisionFunctionPtr(objOuter, objInner);
				}
			}

		}
	}
}

void PhysicsScene::ApplyContactForces(RigidBody* a_actor1, RigidBody* a_actor2, glm::vec2 a_collisionNormal, float a_pen)
{
	if ((a_actor1 && a_actor1->IsTrigger()) || (a_actor2 && a_actor2->IsTrigger()))
	{
		return;
	}

	float body2Mass = a_actor2 ? a_actor2->GetMass() : INT_MAX;
	float body1Factor = body2Mass / (a_actor1->GetMass() + body2Mass);

	a_actor1->SetPosition(a_actor1->GetPosition() - body1Factor * a_collisionNormal * a_pen);

	if (a_actor2)
	{
		a_actor2->SetPosition(a_actor2->GetPosition() + (1 - body1Factor) * a_collisionNormal * a_pen);
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* objPlane, PhysicsObject* objSphere)
{
	return Sphere2Plane(objSphere, objPlane);
}

bool PhysicsScene::Plane2Box(PhysicsObject* objPlane, PhysicsObject* objBox)
{
	Plane* plane = dynamic_cast<Plane*>(objPlane);
	Box* box = dynamic_cast<Box*>(objBox);

	// If these have a value then keep going
	if (box == nullptr || plane == nullptr)
	{
		return false;
	}

	int numContacts = 0;
	//std::vector<glm::vec2> contacts;
	glm::vec2 contact(0, 0);
	float contactV = 0;
	
	// Get a representative point on the plane
	glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

	// check all the corners for a collision with the plain
	for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetExtents().x)
	{
		for (float y = -box->GetExtents().y; y < box->GetWidth(); y += box->GetExtents().y)
		{
			// Get position of corners in world space
			glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
			float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

			// This is the total velocity of the points in world space
			glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
			glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
			// This is the amout of the point velocity into the point
			float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

			// Moving further in, we need to resolve the collision
			if (distFromPlane < 0 && velocityIntoPlane <= 0)
			{
				numContacts++;
				contact += p;
				//contacts.push_back(p);
				contactV += velocityIntoPlane;
			}
		}
	}
	if (numContacts > 0)
	{
		plane->ResolveCollision(box, contact / (float)numContacts);
		return true;
	}
	//for (size_t i = 0; i < contacts.size(); i++)
	//{
	//	plane->ResolveCollision(box, contacts[i]);
	//	
	//}
	//if (numContacts > 0)
	//{
	//	return true;
	//}
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* objSphere, PhysicsObject* objPlane)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Plane* plane = dynamic_cast<Plane*>(objPlane);

	// If these have a value then keep going
	if (sphere == nullptr || plane == nullptr)
	{
		return false;
	}

	glm::vec2 collisionNormal = plane->GetNormal();
	float sphereToPlane = glm::dot(sphere->GetPosition(), collisionNormal) - plane->GetDistance();
	float intersection = sphere->GetRadius() - sphereToPlane;
	float velocityOutOfPlane = glm::dot(sphere->GetVelocity(), collisionNormal);
	if (intersection > 0 && velocityOutOfPlane < 0)
	{
		glm::vec2 contact = sphere->GetPosition() + (collisionNormal * -sphere->GetRadius());
		plane->ResolveCollision(sphere, contact);
		return true;
	}
	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	// If these have a value then keep going
	if (sphere1 == nullptr || sphere2 == nullptr)
	{
		return false;
	}

	float dist = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
	float penetration = sphere1->GetRadius() + sphere2->GetRadius() - dist;
	if (penetration > 0)
	{
		//glm::vec2 contactForce = 0.5f * (dist - (sphere1->GetRadius() + sphere2->GetRadius)) * sphere2->GetPosition()

		sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition()), penetration);
		return true;
	}
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* objSphere, PhysicsObject* objBox)
{
	Sphere* sphere = dynamic_cast<Sphere*>(objSphere);
	Box* box = dynamic_cast<Box*>(objBox);
	// If these have a value then keep going
	if (sphere == nullptr || box == nullptr)
	{
		return false;
	}

	// transfrom the circle into the box's coordinate space
	glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
	glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

	// Find the closest point to the circle's centre on the box
	// by clamping the coordinates in the box-space to the box's extents

	glm::vec2 closestPointOnTheBox = circlePosBox;
	glm::vec2 extent = box->GetExtents();
	if (closestPointOnTheBox.x < -extent.x) closestPointOnTheBox.x = -extent.x;
	if (closestPointOnTheBox.x > extent.x) closestPointOnTheBox.x = extent.x;
	if (closestPointOnTheBox.y < -extent.y) closestPointOnTheBox.y = -extent.y;
	if (closestPointOnTheBox.y > extent.y) closestPointOnTheBox.y = extent.y;

	// Now convert it back into world coordinates
	glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnTheBox.x * box->GetLocalX() + closestPointOnTheBox.y * box->GetLocalY();
	
	glm::vec2 circleToBox = sphere->GetPosition() - closestPointOnBoxWorld;
	float penetration = sphere->GetRadius() - glm::length(circleToBox);
	if (penetration > 0)
	{
		glm::vec2 direction = glm::normalize(circleToBox);
		glm::vec2 contact = closestPointOnBoxWorld;
		box->ResolveCollision(sphere, contact, penetration, &direction);
	}
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject*objBox, PhysicsObject* objPlane)
{
	return Plane2Box(objPlane, objBox);
}

bool PhysicsScene::Box2Sphere(PhysicsObject* objBox, PhysicsObject* objSphere)
{
	return Sphere2Box(objSphere, objBox);
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	// If these have a value then keep going
	if (box2 == nullptr || box1 == nullptr)
	{
		return false;
	}

	glm::vec2 norm(0, 0);
	glm::vec2 contact(0, 0);
	float pen = 0;
	int numContants = 0;
	box1->CheckCorners(*box2, contact, numContants, pen, norm);
	if (box2->CheckCorners(*box1, contact, numContants, pen, norm))
	{
		norm = -norm;
	}
	if (pen > 0)
	{
		box1->ResolveCollision(box2, contact / (float)numContants, pen, &norm);
		return true;
	}
	return false;
}
