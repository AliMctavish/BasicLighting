#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>
#include "TextureLoader.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH  1200

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float horizontal_directions = 0;
float vertical_directions = 0;
float resize = 0;
bool isMoving = true;
float increase = 0;
float increase2 = 0;
float fov = 0;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;



	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		horizontal_directions -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		horizontal_directions += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		vertical_directions += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		vertical_directions -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		resize += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		resize -= 0.01f;


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		fov += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		fov -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		isMoving = false;
	else
		isMoving = true;


}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "the lagacy", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "something went wrong with glad !" << std::endl;
		return -1;
	}


	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_uc* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);



	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);


	stbi_uc* data2 = stbi_load(".png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load the image file" << std::endl;
	}

	stbi_image_free(data2);
	glEnable(GL_DEPTH_TEST);

	//HERE IS THE DRAWING DETAILS
	float vertices[] =
	{
		//POSITIONS   //COLORS   //TEXTURE
		-0.5,-0.5,0,   1,1,1,	 1.0f,1.0f, //0
		-0.5, 0.5,0,   0,1,1,	 1.0f,0.0f,	//1
		 0.5, 0.5,0,   1,0,1,	 0.0f,0.0f,	//2
		 0.5,-0.5,0,   1,1,0,	 0.0f,1.0f,	//3
		 0.5,-0.5,1,   1,0,1,	 1.0f,1.0f,	//4
		 0.5,0.5,1,    1,1,0,	 1.0f,0.0f,	//5
		 -0.5,0.5,1,   1,0,1,	 0.0f,0.0f,	//6
		 -0.5,-0.5,1,  0,1,1,	 0.0f,1.0f,	//7
		 -0.5,0.5,1,   1,1,0,	 1.0f,1.0f,	//8
		 0.5,0.5,1,    1,1,1,	 0.0f,1.0f,	//9
		 -0.5,-0.5,1,  0,1,1,	 1.0f,0.0f,	//10
		 0.5,-0.5,1,  1,1,0,	 0.0f,0.0f,	//11
	};
	unsigned int indecies[] =
	{
		0,1,2,
		2,0,3,
		3,2,4,
		4,2,5,
		0,1,6,
		0,6,7,
		1,2,8,
		8,2,9,
		7,6,5,
		5,7,4,
		0,3,10,
		10,3,11,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int EAO;
	glGenBuffers(1, &EAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), &indecies, GL_STATIC_DRAW);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader shader("VertexShader.shader", "FragmentShader.shader");

	shader.UnBind();
	glUniform1i(glGetUniformLocation(shader.shader_program, "textureFrag"), 0); // set it manually
	shader.setInt("textureFrag2", 1); // or with shader class
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* Make the window's context current */

	/* Loop until the user closes the window */


	std::vector<glm::vec3> cubes;

	for (unsigned int i = 1; i < 20; i++)
		for (unsigned int j = 1; j < 20; j++)
			for (unsigned int k = 1; k < 20; k++)
				cubes.push_back(glm::vec3(i + j / k * i ,j / i + j + k, k + j / i));



	glm::vec3 models[] =
	{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f),
	glm::vec3(0.5f,  0.0f, -2.5f),
	glm::vec3(1.5f,  -3.2f, -1.5f),
	glm::vec3(-1.3f,  -3.0f, -4.5f)
	};

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Bind();


		if (isMoving)
			increase += 0.01f;


		glBindVertexArray(VAO);

		

		for (unsigned int i = 0; i < cubes.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubes[i]);

			int modelLoc = glGetUniformLocation(shader.shader_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glm::mat4 view;
		const float radius = 10.0f;
		float camX = sin(glfwGetTime() * 0.01) * radius ;
		float camZ = cos(glfwGetTime() * 0.01) * radius ;

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -30.0f));
		view = glm::lookAt(cameraPos,  cameraFront, cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 1200.f/ 800.f, 0.1f, 100.0f);


		int viewLoc = glGetUniformLocation(shader.shader_program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projectionLoc = glGetUniformLocation(shader.shader_program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		shader.set4Float("colorTest", 1, 1, 0, 1);
		//shader.setFloat("Xoffset", horizontal_directions);
		//shader.setFloat("Yoffset", vertical_directions);
		//shader.setFloat("Zoffset", resize);
		shader.setInt("textureFrag", 0); // or with shader class
		shader.setInt("textureFrag2", 1); // or with shader class

		processInput(window);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}