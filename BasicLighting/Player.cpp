#include "Player.h"

Player::Player()
{
   Position = glm::vec3(3.0f, 3.0f, 3.0f);
   CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
   CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
   CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
}


void Player::SetMatrix()
{
	glm::mat4 view;
	const float radius = 10.0f;
	float camX = sin(glfwGetTime() * 0.01) * radius;
	float camZ = cos(glfwGetTime() * 0.01) * radius;
	view = glm::translate(view, glm::vec3(-10.0f, 10.0f, 20.0f));
	view = glm::lookAt(Position, CameraFront + Position, CameraUp);
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(75.0f), 1200.f / 800.f, 0.1f, 100.0f);

	View = view;
	Projection = projection;
}




Player::~Player()
{
}
