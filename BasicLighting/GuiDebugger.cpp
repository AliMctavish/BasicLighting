#include "GuiDebugger.h"


GuiDebugger::GuiDebugger()
{
	m_Window = 0;
}

void GuiDebugger::SetWindow(GLFWwindow* window)
{
	m_Window = window;
}

void GuiDebugger::Debugger(std::vector<Light>& lights, std::vector<Cube>& cubes, Shader& shader, Shader& lightShader, std::string& frames, bool& gameStarted)
{
	StartFrames();
	Begin("Objects Coordinates");

	for (int i = 0; i < cubes.size(); i++)
	{
		ImGui::Text(cubes[i].GetName().c_str());
		ImGui::PushID(cubes[i].cubeId);
		ImGui::SliderFloat("Move2 On X", &cubes[i].Position.x, -50, 50, "%.3f", 0);
		ImGui::SliderFloat("Move2 On Y", &cubes[i].Position.y, -50, 50, "%.3f", 0);
		ImGui::SliderFloat("Move2 On Z", &cubes[i].Position.z, -50, 50, "%.3f", 0);
		ImGui::SliderFloat("Rsizex object", &cubes[i].Size.x, 0, 100, "%.3f", 0);
		ImGui::SliderFloat("Rsizey object", &cubes[i].Size.y, 0, 100, "%.3f", 0);
		ImGui::SliderFloat("Rsizez object", &cubes[i].Size.z, 0, 100, "%.3f", 0);
		ImGui::SliderFloat("Rotate on x Axis", &cubes[i].rotateX, 0, 10, "%.3f", 0);
		ImGui::SliderFloat("Rotate on z Axis", &cubes[i].rotateY, 0, 10, "%.3f", 0);
		ImGui::Text("Object Material");
		ImGui::ColorEdit3(cubes[i].GetName().c_str(), cubes[i].Color, 0);
		ImGui::SliderFloat2("Ambiant", &cubes[i].material.Ambiant[0], 0, 10, "%.3f", 0);
		ImGui::SliderFloat2("Diffuse", &cubes[i].material.Diffuse[0], 0, 10, "%.3f", 0);
		ImGui::SliderFloat2("Specular", &cubes[i].material.Specular[0], 0, 10, "%.3f", 0);
		ImGui::SliderFloat("Shininess", &cubes[i].material.shininess, 0, 100, "%.3f", 0);
		if (ImGui::Button("Delete", Button_Size)) { cubes.erase(cubes.begin() + i); }
		ImGui::PopID();
	}

	End();

	Begin("Object Coordinates2");

	for (int i = 0; i < lights.size(); i++)
	{
		ImGui::Text(lights[i].GetName().c_str());
		ImGui::PushID(lights[i].objectId);
		ImGui::ColorEdit3("Object2 Color", lights[i].Color, 0);

		ImGui::SliderFloat("Move On X", &lights[i].Position.x, -50, 50, "%.3f", 0);
		ImGui::SliderFloat("Move On Y", &lights[i].Position.y, -100, 100, "%.3f", 0);
		ImGui::SliderFloat("Move On Z", &lights[i].Position.z, -50, 50, "%.3f", 0);

		ImGui::SliderFloat("2Rsize object", &lights[i].Size.x, 0, 10, "%.3f", 1);
		ImGui::SliderFloat("2Rotate on x Axis", &lights[i].rotateX, 0, 10, "%.3f", 1);
		ImGui::SliderFloat("2Rotate on y Axis", &lights[i].rotateY, 0, 10, "%.3f", 1);
		ImGui::SliderFloat("2Rotate on z Axis", &lights[i].rotateZ, 0, 10, "%.3f", 1);
		if (ImGui::Button("Delete", Button_Size)) { lights.erase(lights.begin() + i); }
		ImGui::PopID();

	}
	End();
	Begin("Scene Settings");
	if (ImGui::Button("Show WireFrame"))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (ImGui::Button("Hide WireFrame"))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	End();

	Begin("Select Cube Texture");

	if (ImGui::Button("Wall", Button_Size))
	{
		selectedTexture = FileManager::SelectTextureFile(WallTexturePath);
	}
	if (ImGui::Button("Ground", Button_Size))
	{
		selectedTexture = FileManager::SelectTextureFile(GroundTexturePath);
	}
	if (ImGui::Button("Box", Button_Size))
	{
		selectedTexture = FileManager::SelectTextureFile(BoxTexturePath);
	}

	End();
	Begin("World Settings");

	ImGui::Text(frames.c_str());

	if (ImGui::Button("Create Cube", Button_Size))
	{
		Cube cube;
		cube.SetShader(shader);
		cube.SetName("test" + std::to_string(cubes.size()));
		cube.texture.SetTexture(selectedTexture, 1);
		cubes.push_back(cube);
	}

	if (ImGui::Button("Create Light", Button_Size))
	{
		Light light;
		light.SetProgram(lightShader.shader_program);
		light.SetName("object" + std::to_string(cubes.size()));
		lights.push_back(light);
	}
	if (ImGui::Button("Start Game", Button_Size))
		gameStarted = true;


	if (ImGui::Button("Select Map", Button_Size))
	{
		m_MapSelector = true;
	}
	if (m_MapSelector)
	{
		Begin("Select Map");
		if (ImGui::Selectable("Level1", false, 0, Selector_Size))
			selectedMap = "Level1";
		else if (ImGui::Selectable("Level2", false, 0, Selector_Size))
			selectedMap = "Level2";
		else if (ImGui::Selectable("Level3", false, 0, Selector_Size))
			selectedMap = "Level3";
		else if (ImGui::Selectable("Level4", false, 0, Selector_Size))
			selectedMap = "Level4";
		else if (ImGui::Selectable("Level5", false, 0, Selector_Size))
			selectedMap = "Level5";

		if (ImGui::Button("Select", Button_Size))
		{
			FileManager::LoadFile(lights, cubes, lightShader, shader, selectedMap);
			m_MapSelector = false;
		}
		if (ImGui::Button("Save Map", Button_Size))
		{
			FileManager::SaveFile(lights, cubes, selectedMap);
			m_MapSelector = false;
		}
		End();
	}
	//ImGui::ColorEdit3("BackgroundColor", bgColor, 0);
	End();
	EndFrames();
}


GuiDebugger::~GuiDebugger()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GuiDebugger::StartFrames()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GuiDebugger::EndFrames()
{
	ImGui::Render();
	ImGui::EndFrame();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiDebugger::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void GuiDebugger::Begin(const char* windowTitle)
{
	ImGui::Begin(windowTitle);
}

void GuiDebugger::End()
{
	ImGui::End();
}

void GuiDebugger::Display()
{
	ImGui::Text("this is a text inside opengl");
}

