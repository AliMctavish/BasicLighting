#include "Renderer.h"
#include "Controllers.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 lightPos = glm::vec3(1, 3, 2);

void Renderer::Init()
{
	_window = nullptr;
}

Renderer::Renderer(GLFWwindow* window)
{
	Init();
	_window = window;
	_gui.SetWindow(_window);
}

void Renderer::Initialize()
{
	shader.SetShaders("VertexShader.shader", "FragmentShader.shader");
	lightShader.SetShaders("LightVertexShader.shader", "LightFragmentShader.shader");

	vertexArray.Bind();
	vertexBuffer.Bind();
	vertexBuffer.SetCubeWithNormalsAttributes();

	vertexArray2.Bind();
	vertexBuffer2.Bind();

	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	cube.SetProgram(shader.shader_program);
	cube.SetName("Brick");

	cube2.SetProgram(lightShader.shader_program);
	cube2.SetName("Another brick");

	shader.UnBind();
	lightShader.UnBind();
	_gui.Init();
}

float bgColor[] = {1,1,1};
void Renderer::Update()
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;


	//CAMERA STUFF SHOULD BE ADDED SOMEWHERE ELSE OUT OF HERE
	glm::mat4 view;
	const float radius = 10.0f;
	float camX = sin(glfwGetTime() * 0.01) * radius;
	float camZ = cos(glfwGetTime() * 0.01) * radius;
	view = glm::translate(view, glm::vec3(0.0f, -8.0f, 20.0f));
	view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1200.f / 800.f, 0.1f, 100.0f);
	//CAMERA STUFF SHOULD BE ADDED SOMEWHERE ELSE OUT OF HERE


	shader.Bind();
	cube.SetLocation(glm::vec3(cube.xCoord, cube.yCoord, cube.zCoord));
	cube.Resize(glm::vec3(cube.size, cube.size, cube.size));
	cube.RotateX(cube.rotateX);
	cube.RotateY(cube.rotateY);
	cube.RotateZ(cube.rotateZ);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", cameraPos);
	shader.SetMat4("model",cube.GetModel());
	cube.SetColor("customColor");
	vertexArray.Bind();
	cube.Draw();


	lightShader.Bind();
	cube2.SetLocation(lightPos);
	cube2.Resize(glm::vec3(cube2.size, cube2.size, cube2.size));
	cube2.RotateX(cube2.rotateX);
	cube2.RotateY(cube2.rotateY);
	cube2.RotateZ(cube2.rotateZ);
	lightShader.SetMat4("view", view);
	lightShader.SetMat4("projection", projection);
	lightShader.SetMat4("model", cube2.GetModel());
	cube2.SetColor("objectColor");
	vertexArray2.Bind();
	cube2.Draw();


	//glfwSetCursorPosCallback(window, mouse_callback);
	Debugger();

	processInput(_window);
	/* Swap front and back buffers */
	glfwSwapBuffers(_window);
	/* Poll for and process events */
	glfwPollEvents();
}

void Renderer::Debugger()
{
	_gui.StartFrames();

	_gui.Begin("Object Coordinates");
	ImGui::Text(cube.GetName());
	if (ImGui::Button("xCoord right", ImVec2(150, 20)))
		cube.xCoord++;
	if (ImGui::Button("xCoord left", ImVec2(150, 20)))
		cube.yCoord--;
	if (ImGui::Button("zCoord forward", ImVec2(150, 20)))
		cube.zCoord++;
	if (ImGui::Button("zCoord backward", ImVec2(150, 20)))
		cube.zCoord--;
	if (ImGui::Button("yCoord up", ImVec2(150, 20)))
		cube.yCoord++;
	if (ImGui::Button("yCoord down", ImVec2(150, 20)))
		cube.yCoord--;
	ImGui::ColorEdit3("Object1 Color", cube.Color, 0);
	_gui.End();

	_gui.Begin("Object Orientation");
	ImGui::SliderFloat("Rsize object", &cube.size, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("Rotate on x Axis", &cube.rotateX, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("Rotate on y Axis", &cube.rotateY, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("Rotate on z Axis", &cube.rotateZ, 0, 10, "%.3f", 0);
	_gui.End();

	_gui.Begin("Object Coordinates2");

	ImGui::Text(cube2.GetName());
	if (ImGui::Button("2xCoord right", ImVec2(150, 20)))
		lightPos.x++;
	if (ImGui::Button("2xCoord left", ImVec2(150, 20)))
		lightPos.x--;
	if (ImGui::Button("2zCoord forward", ImVec2(150, 20)))
		lightPos.y++;
	if (ImGui::Button("2zCoord backward", ImVec2(150, 20)))
		lightPos.y--;
	if (ImGui::Button("2yCoord up", ImVec2(150, 20)))
		lightPos.z++;
	if (ImGui::Button("2yCoord down", ImVec2(150, 20)))
		lightPos.z--;

	ImGui::ColorEdit3("Object2 Color", cube2.Color, 0);
	_gui.End();

	_gui.Begin("Object Orientation2");
	ImGui::SliderFloat("2Rsize object", &cube2.size, 0, 10, "%.3f", 1);
	ImGui::SliderFloat("2Rotate on x Axis", &cube2.rotateX, 0, 10, "%.3f", 1);
	ImGui::SliderFloat("2Rotate on y Axis", &cube2.rotateY, 0, 10, "%.3f", 1);
	ImGui::SliderFloat("2Rotate on z Axis", &cube2.rotateZ, 0, 10, "%.3f", 1);

	_gui.End();

	_gui.Begin("World Settings");

	ImGui::ColorEdit3("BackgroundColor", bgColor, 0);

	_gui.End();

	_gui.EndFrames();
}
