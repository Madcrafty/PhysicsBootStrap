#include "Club.h"
#include "Input.h"

Club::Club(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height)
	: Box(a_position, a_velocity, a_rotation, a_mass, a_width, a_height)
{
}

Club::Club(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_colour)
	: Box(a_position, a_velocity, a_rotation, a_mass, a_width, a_height, a_colour)
{
}

Club::~Club()
{
}

void Club::FixedUpdate(glm::vec2 a_gravity, float a_timestep)
{
	//aie::Input* input = aie::Input::getInstance();
	//if (input->isMouseButtonDown(0))
	//{
	//	int xScreen, yScreen;
	//	input->getMouseXY(&xScreen, &yScreen);
	//	glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
	//	aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0.3));

	//}
}
