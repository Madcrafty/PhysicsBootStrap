#pragma once
#include "Box.h"
class Club : public Box
{
public:
	Club(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height);
	Club(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour);
	~Club();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timestep);

protected:
};

