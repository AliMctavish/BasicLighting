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
	_machine.SetShader(lightShader);
	//_machine.SetModel(modelLoader);
	_gate.SetShader(lightShader);
	//_gate.SetModel(modelLoader);
	font.SetView(_player.Projection);
	_player.SetShader(shader);

	glfwSwapInterval(0);

	FileManager::LoadFile(lights, cubes, keys, enemies, _machine, _gate, lightShader, shader, modelShader, modelLoader, "Level1");
}

void Renderer::Update(std::string& deltaTime)
{
	_player.Update();
	testSprite.Update();
	_gate.Update();

	//not doing anything
	//std::cout << deltaTime << std::endl;


	if (glm::distance(_player.Position, _gate.Position) <= 20)
	{
		_player.inRangeOfGateObject = true;
		if (_player.hasKey)
			if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
			{
				Key IndexedKey = keys[_player.GetPickedKeyIndex()];
				if (_gate.CheckKeyColor(IndexedKey.Color[0], IndexedKey.Color[1], IndexedKey.Color[2]))
				{
					_player.NumberOfKeys+=1;
					_player.hasKey = false;
					_player.CorrectKey = true;
					keys.erase(keys.begin() + _player.GetPickedKeyIndex());
				}
				else
					_player.MaybePickedKeyValid = false;
			}
	}
	else {
		_player.inRangeOfGateObject = false;
		_player.CorrectKey = false;
		_player.MaybePickedKeyValid = true;
	}

	if (glm::distance(_player.Position, _machine.Position) <= 10)
	{
		_player.inRangeOfMachineObject = true;
		if (_player.hasLight && _machine.m_Lights.size() != 2)
		{
			if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
			{
				_machine.AddLight(lights[_player.GetPickedLightIndex()]);
				lights[_player.GetPickedLightIndex()].ResetValues();
				//lights.erase(lights.begin() + _player.GetPickedLightIndex());
				_player.hasLight = false;
			}
		}

		if (_player.hasKey)
			if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
			{
				_machine.AssignToKey(keys[_player.GetPickedKeyIndex()]);
			}


		if (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS && !_player.hasKey)
			_machine.ResetLights();
	}
	else
		_player.inRangeOfMachineObject = false;

	_machine.Update();

	//checks if the player has collided with a ground (cube) if so then this 
	//loop will stop and the player will save the collided cube 
	if (!_player.CollidedWithCube)
		for (int i = 0; i < cubes.size(); i++)
		{
			if (_physics.CheckCollision(cubes[i], _player))
			{
				_player.AssignCollidedCube(i);
				break;
			}
		}
	//if the player has collided with a cube it will update the checker only 
	//on this cube ! 
	if (_player.grounded && _player.CollidedWithCube)
		if (!_physics.CheckCollision(cubes[_player.GetCollidedCubeIndex()], _player))
			_player.CollidedWithCube = false;

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].Update();
		if (gameStarted)
			enemies[i].MoveTowardsPlayer(_player);

		for (int j = 0; j < lights.size(); j++)
		{
			if (enemies.size() > 0 && lights.size() > 0 && lights[j].isPushing)
				if (_physics.IsCollidedTest(enemies[i].Position, lights[j].Position, glm::vec3(5, 5, 5)))
				{
					enemies.erase(enemies.begin() + i);
					lights[i].ResetValues();
					//lights.erase(lights.begin() + j);
				}
		}
	}

	if (gameStarted == true)
	{
		_physics.UpdateGravity(_player);

		//will stop the object generator only after i make a mode for waves 
		//objectGenerator.SetModelLoader(modelLoader);
		//objectGenerator.GenerateEnemy(enemies, modelShader);
		//objectGenerator.GenerateLight(lights, lightShader);
	}

	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].Update(_player, _window);

		if (!_player.hasKey)
			if (_physics.IsCollided(keys[i].Position, _player.Position, glm::vec3(3, 3, 3)))
			{
				_player.inRangeOfKeyObject = true;
				if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
				{
					keys[i].isPickedUp = true;
					_player.hasKey = true;
					_player.inRangeOfKeyObject = false;
					_player.AssignPickedKey(i);
				}
			}
	}

	for (int i = 0; i < lights.size(); i++)
	{
		// clean the code soon

		if (!_player.hasLight)
			if (glm::distance(lights[i].Position, _player.Position) <= 10)
			{
				if (!lights[i].isPushing && !lights[i].isPickedUp)
					_player.inRangeOfLightObject = true;

				if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS && !_player.hasLight)
				{
					_player.hasLight = true;
					lights[i].isPickedUp = true;
					_player.AssignPickedLight(i);
				}
			}

		lights[i].Update(_player, _window);

		if (lights[i].isPushing)
		{
			lights[i].Push();
			if (glm::distance(lights[i].Position, _player.Position) > 300)
			{
				lights[i].ResetValues();
				continue;
			}
		}
		//this part of the code will override the update method of the light 
		//in the matrix part
		//when you are not picking up the object ! 
		if (!lights[i].isPickedUp && !lights[i].isPushing)
			lights[i].SinMove();
	}


	for (Cube& cube : cubes)
	{
		cube.Update();

		for (Light& light : lights)
			if (light.isPushing)
				if (_physics.IsCollidedTest(cube.Position, light.Position, cube.Size))
				{
					light.isPushing = false;
					light.isPickedUp = false;
				}

	}

	processInput(_window, _player);
}

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	testSprite.Draw();

	modelShader.Bind();
	modelShader.SetMat4("view", _player.View);
	modelShader.SetMat4("projection", _player.Projection);


	for (Enemy& enemy : enemies)
		enemy.Draw(modelLoader);

	modelShader.UnBind();

	vertexArray.Bind();
	shader.Bind();
	_player.Draw();

	for (int i = 0; i < lights.size(); i++)
	{
		shader.setVec3("lightPos[" + std::to_string(i) + ']', lights[i].Position);
		shader.setVec3("lightColor[" + std::to_string(i) + ']', glm::vec3(lights[i].Color[0], lights[i].Color[1], lights[i].Color[2]));
	}

	shader.setInt("LightCount", lights.size());

	for (Cube& cube : cubes)
		cube.Draw();

	shader.UnBind();

	lightShader.Bind();
	lightShader.SetMat4("view", _player.View);
	lightShader.SetMat4("projection", _player.Projection);

	for (Light& light : lights)
		light.Draw(modelLoader);

	_gate.Draw(modelLoader);
	_machine.Draw(modelLoader);

	for (Key& key : keys)
		key.Draw(modelLoader);

	lightShader.UnBind();

	font.Draw("number of enemies : " + std::to_string(enemies.size()), -0.8, 0.8, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
	font.Draw("Number Of Keys Left : " + std::to_string(_gate.RequiredColors.size()) , -0.8, 0.9, 0.001f, glm::vec3(0.9, 0.9f, 0.1f));


	if (_player.inRangeOfKeyObject)
		font.Draw("Press 'E' to collect", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_player.inRangeOfGateObject)
	{
		if (!_player.hasKey && !_player.CorrectKey)
			font.Draw("You should provide a key to open the gate", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
		else if(_player.hasKey && _player.MaybePickedKeyValid)
			font.Draw("Press 'E' to put the key", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

		if(!_player.MaybePickedKeyValid)
			font.Draw("Key color not match !", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

		if (_player.CorrectKey)
		{
			font.Draw("Well Done!", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
			font.Draw(" ' " + std::to_string(_gate.RequiredColors.size()) + " ' " + " keys left !", -0.4, 0.6, 0.001f, glm::vec3(0.1, 0.8f, 0.2f));
		}
	}

	if (_player.inRangeOfLightObject)
		font.Draw("Press 'E' to pick up", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_player.inRangeOfMachineObject && !_player.hasLight && _machine.m_Lights.size() == 0)
		font.Draw("You need light to use this machine", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_player.inRangeOfMachineObject && _player.hasKey)
		font.Draw("Press 'E' to modify key color", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_player.inRangeOfMachineObject && _player.hasLight)
		font.Draw("Press 'E' to plug in the light", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));

	if (_machine.m_Lights.size() == 2 && _player.inRangeOfMachineObject && !_player.hasKey)
	{
		font.Draw("The machine is full", -0.4, 0.4, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
		font.Draw("Press 'R' to reset the machine", -0.4, 0.6, 0.001f, glm::vec3(0.5, 0.8f, 0.2f));
	}

	if (gameStarted == false)
	{
		//after adding this function it gave me the ability to control the mouse callback !?
		glfwSetCursorEnterCallback(_window, GLFW_FALSE);
		glfwSetCursorPosCallback(_window, NULL);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		_gui.Debugger(lights, cubes, enemies, keys, shader, lightShader, modelShader, modelLoader, _machine, _gate, gameStarted);
		_gui.SetupImGuiStyle(true, 1);
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