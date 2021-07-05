#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glfwUtil.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void testFunc(GLFWwindow* window, int width, int height);


int main()
{
	auto window = initGlfw();

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, testFunc);


	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void testFunc(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
}