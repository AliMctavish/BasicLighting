#include "Physics.h"

Physics::Physics()
{
	_window = nullptr;
}

void Physics::SetVariables(GLFWwindow* window, Player& player)
{
	_window = window;
	_player = player;
}


bool Physics::CheckCollision(Cube& cube, Player& player)
{
	if (IsCollided(cube.Position, player.Position, cube.Size + glm::vec3(0, 2, 0)))
	{
		float distance = player.Position.y - cube.Position.y;
		player.Position.y = cube.Position.y + distance;
		//implement the sides collitions also ! 

		player.isJumping = false;
		acceleration = 2;

		return player.grounded = true;
	}
	else
		return player.grounded = false;
}

void Physics::UpdateGravity(Player& player)
{
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		player.isJumping = true;

	if (!player.grounded && !player.isJumping)
	{
		player.Position -= player.CameraUp;

		//think about how to solve such case plz !! 
		player.SetMatrix();
	}



	if (player.isJumping)
	{
		player.Position += player.CameraUp * acceleration;
		acceleration -= 0.08f;

		//basically this is a silly solution 
		//the problem is with the matrix of the player translations
		//so when you change the position of the player the matrix still the same
		//so the light will apear to offset the position of the jump
		player.SetMatrix();
	}
}


bool Physics::IsCollided(glm::vec3& object1, glm::vec3& object2, glm::vec3 sizeObject1)
{
	int object1XX = object1.x + sizeObject1.x + 1;
	int object1X = object1.x;
	int object1YY = object1.y + sizeObject1.y + 5;
	int object1Y = object1.y;
	int object1ZZ = object1.z + sizeObject1.z;
	int object1Z = object1.z;

	int object2XX = object2.x + 3;
	int object2X = object2.x;
	int object2YY = object2.y + 2;
	int object2Y = object2.y;
	int object2ZZ = object2.z + 2;
	int object2Z = object2.z;

	return object2Y < object1YY&& object1Y < object2YY&& object2X < object1XX
		&& object1X < object2XX&& object2Z < object1ZZ&& object1Z < object2ZZ;
}


bool Physics::IsCollidedTest(glm::vec3& object1, glm::vec3& object2, glm::vec3 sizeObject1)
{
	int object1XX = object1.x + sizeObject1.x + 1;
	int object1X = object1.x;
	int object1YY = object1.y + sizeObject1.y + 2;
	int object1Y = object1.y;
	int object1ZZ = object1.z + sizeObject1.z;
	int object1Z = object1.z;

	int object2XX = object2.x + 3;
	int object2X = object2.x;
	int object2YY = object2.y + 2;
	int object2Y = object2.y;
	int object2ZZ = object2.z + 2;
	int object2Z = object2.z;

	return object2Y < object1YY&& object1Y < object2YY&& object2X < object1XX
		&& object1X < object2XX&& object2Z < object1ZZ&& object1Z < object2ZZ;
}




Physics::~Physics()
{
}

