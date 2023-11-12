#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"


//MOUSE CONTROLLERS
bool firstMouse = false;
float lastX = 400, lastY = 300;
float yaw = 1;
float pitch = 1;
//CAMERA STUFF IN HERE
float horizontal_directions = 0;
float vertical_directions = 0;
float resize = 2;
bool isMoving = true;
float fov = 0;
glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}


void processInput(GLFWwindow* window, float deltaTime)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	const float cameraSpeed = 0.05f; // adjust accordingly
	const float rotaionSpeed = 0.01f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraRight, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraRight, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw -= 0.5;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw += 0.5;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pitch += 0.5;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= 0.5;


	//CAMERA FIELD OF VIEW 
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		fov += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		fov -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		isMoving = false;
	else
		isMoving = true;

}

//class Controllers {
//public :
//	Controllers();
//	//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//	//static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//	//void processInput(GLFWwindow* window, float deltaTime);
//};


