#include "Renderer.h"
#include "Controllers.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Renderer::Init()
{
	_window = nullptr;
}

Renderer::Renderer(GLFWwindow* window)
{
	Init();
	_window = window;
	_physics.SetVariables(_window, _player);
	_gui.SetWindow(_window);
	modelLoader.InitializeModels();
}

void Renderer::Initialize()
{
	shader.SetShaders("VertexShader.shader", "FragmentShader.shader");
	lightShader.SetShaders("LightVertexShader.shader", "LightFragmentShader.shader");
	modelShader.SetShaders("ModelVertexShader.shader", "ModelFragmentShader.shader");

	testSprite.SetTexture("Assests/aim.png");

	vertexArray.Bind();
	vertexBuffer.Bind();
	vertexArray.UnBind();

	glEnable(GL_DEPTH_TEST);


	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	_gui.Init();
	font.SetView(_player.Projection);

	glfwSwapInterval(0);

	FileManager::LoadFile(lights, cubes, keys, lightShader, shader, "Level1");

}

void Renderer::Update()
{
	//_player.Update();
	_player.OscillateOnMoving();
	_player.SetMatrix();

	testSprite.Update();

	for (Cube& cube : cubes)
	{
		cube.Update();

		if (_physics.CheckCollision(cube, _player))
			break;
	}

	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].Update();

		if (_physics.IsCollided(keys[i].Position, _player.Position, keys[i].Size = glm::vec3(3,3,3)))
		{
			_player.inRangeOfKeyObject = true;
			if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
			{
				_player.inRangeOfKeyObject = false;
				_player.NumberOfKeys += 1;
				keys.erase(keys.begin() + i);
			}
		}
		else
			_player.inRangeOfKeyObject = false;
	}


	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].Update();
		if (gameStarted)
			enemies[i].MoveTowardsPlayer(_player);

		for (int j = 0; j < lights.size(); j++)
		{
			if (enemies.size() > 0 && lights.size() > 0)
				if (_physics.IsCollidedTest(enemies[i].Position, lights[j].Position, enemies[i].Size) && lights[j].isPushing)
				{
					enemies.erase(enemies.begin() + i);
					lights.erase(lights.begin() + j);
				}
		}
	}

	if (gameStarted == true)
	{
		_physics.UpdateGravity(_player);
		objectGenerator.GenerateEnemy(enemies, modelShader);
		objectGenerator.GenerateLight(lights, lightShader);
	}

	for (int i = 0; i < lights.size(); i++)
	{
		// clean the code soon
		_physics.CheckLightCollision(lights[i], _player);
		lights[i].Update(_player, _window);

		if (lights[i].isPushing)
		{
			lights[i].Push();
			if (glm::distance(lights[i].Position, _player.Position) > 300)
			{
				lights.erase(lights.begin() + i);
				continue;
			}
		}

		if (_physics.CheckLightCollision(lights[i], _player) && !lights[i].isPickedUp && !lights[i].isPushing)
			_player.inRangeOfLightObject = true;
		else
			_player.inRangeOfLightObject = false;


		//this part of the code will override the update method of the light 
		//in the matrix part
		//when you are not picking up the object ! 
		if (!lights[i].isPickedUp && !lights[i].isPushing)
			lights[i].SinMove();
	}

	processInput(_window, _player);
}


std::string m_TextCounter;
float m_Counter;

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	testSprite.Draw();

	//why the fuck there is nothing showing on the screen ?????????????
	//okay .... basically you should send the view matrix for the camera !!!-_-

	modelShader.Bind();

	//abstract this motherfucker plz !.
	modelShader.SetMat4("view", _player.View);
	modelShader.SetMat4("projection", _player.Projection);

	for (Enemy& enemy : enemies)
		enemy.Draw(modelLoader);

	modelShader.UnBind();

	vertexArray.Bind();
	shader.Bind();

	shader.SetMat4("view", _player.View);
	shader.SetMat4("projection", _player.Projection);
	shader.setVec3("viewPos", _player.Position);

	//too many for loops for testing perposses idk how to write perpoesrpes
	for (int i = 0; i < lights.size(); i++)
	{
		shader.setVec3("lightPos[" + std::to_string(i) + ']', lights[i].Position);
		shader.setVec3("lightColor[" + std::to_string(i) + ']', glm::vec3(lights[i].Color[0], lights[i].Color[1], lights[i].Color[2]));
	}

	shader.setInt("LightCount", lights.size());


	for (Cube& cube : cubes)
	{
		for (Light& light : lights)
		{
			//hmmmm is this the way i should work with ? 
			if (_physics.IsCollidedTest(cube.Position, light.Position, cube.Size))
				light.isPushing = false;
		}
		cube.Draw();
	}
	shader.UnBind();

	lightShader.Bind();
	lightShader.SetMat4("view", _player.View);
	lightShader.SetMat4("projection", _player.Projection);

	for (Light& light : lights)
		light.Draw(modelLoader);

	for (Key& key : keys)
		key.Draw(&modelLoader);

	lightShader.UnBind();


	font.Draw("number of enemies : " + std::to_string(enemies.size()), -0.8, 0.8, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
	font.Draw("Number Of Keys Left : " + std::to_string(_player.NumberOfKeys) + "/" + std::to_string(keys.size()), -0.8, 0.9, 0.001f, glm::vec3(0.9, 0.9f, 0.1f));

	if (_player.inRangeOfKeyObject)
		font.Draw("Press 'E' to collect", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_player.inRangeOfLightObject)
		font.Draw("Press 'E' to pick up", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));


	if (gameStarted == false)
	{
		//after adding this function it gave me the ability to control the mouse callback !?
		glfwSetCursorEnterCallback(_window, GLFW_FALSE);
		glfwSetCursorPosCallback(_window, NULL);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		_gui.Debugger(lights, cubes, enemies, keys, shader, lightShader, modelShader, gameStarted);
		_gui.SetupImGuiStyle(true, 1);


		if (glfwGetKey(_window, GLFW_KEY_P) == GLFW_PRESS)
		{
			Cube cube;
			cube.SetShader(shader);
			cube.Position = _player.Position;
			cube.SetName("test" + std::to_string(cubes.size()));
			cube.SetTextureData(1);
			cubes.push_back(cube);
		}
		if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS)
		{
			Light light;
			light.SetShader(lightShader);
			light.Position = _player.Position;
			light.SetName("test" + std::to_string(cubes.size()));
			lights.push_back(light);
		}
	}
	else
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(_window, mouse_callback);

		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			gameStarted = false;
		//glfwSetWindowShouldClose(window, true);
	}
	/* Swap front and back buffers */
	glfwSwapBuffers(_window);
	/* Poll for and process events */
	glfwPollEvents();
}

