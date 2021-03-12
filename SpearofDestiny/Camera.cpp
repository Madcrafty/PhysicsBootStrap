#include "Camera.h"
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_theta = 0;
	m_phi = 0;
}

void Camera::Update(float a_deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	// Calculate the forward, right and up axis for the camera
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0,1,0);

	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		m_position += up * m_speed * a_deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
	{
		m_position -= up * m_speed * a_deltaTime;
	}	
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_position += right * m_speed * a_deltaTime;
	}	
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_position -= right * m_speed * a_deltaTime;
	}	
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_position += forward * m_speed * a_deltaTime;
	}	
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_position -= forward * m_speed * a_deltaTime;
	}

	// Get the current position of the mouse
	float mX = input->getMouseX();
	float mY = input->getMouseY();
	const float turnSpeed = glm::radians(180.f);

	// If the right button is down, increment the thea and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += turnSpeed * (mX - m_lastMouseX) * m_sensitivity * a_deltaTime;
		m_phi += turnSpeed * (mY - m_lastMouseY) * m_sensitivity * a_deltaTime;
	}

	// Now store the frames last calues for the next
	m_lastMouseX = mX;
	m_lastMouseY = mY;
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR)* glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0,1,0));
}

glm::mat4 Camera::GetProjectionMatrix(float a_width, float a_height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, a_width / a_height, 1.0f, 1000.f);
}
