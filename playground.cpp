// Include Standard
#include <iostream>
#include <vector>
#include <exception>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.cpp>
#include <common/objloader.cpp>
#include <common/vboindexer.hpp>

#include "../../../ColourGenerator.h"


void wireFrame(GLFWwindow* window);
void initialize(int xPos, int yPos);
void lightColourChange(GLFWwindow* window, glm::vec3& lightColorVec);

int main(void)
{
	initialize(1024, 768);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//////////
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Get a handle for our "myTextureSampler" uniform
	//GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	glm::vec3 lightColorVec(1.0f, 1.0f, 1.0f);
	int lightColour = glGetUniformLocation(programID, "LightColor");
	//////////


	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("Flowers.obj", vertices, uvs, normals);

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		wireFrame(window);

		//std::vector<GLfloat> g_color_buffer_data = colourGenerator.colourChange(vertices.size());
		//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data[0]) * g_color_buffer_data.size(), g_color_buffer_data.data(), GL_STATIC_DRAW);

		// Use our shader
		glUseProgram(programID);

		
		glUniform3f(lightColour, lightColorVec.x, lightColorVec.y, lightColorVec.z);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//////
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


		//Light Pos
		//glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glm::vec3 lightPos = getCameraPosition();
		//int lightPos = glGetUniformLocation(programID, "LightPosition_worldspace");
	
		// Bind our texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE0);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(TextureID, 0);


		lightColourChange(window, lightColorVec);
		//////

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

}

void lightColourChange(GLFWwindow* window, glm::vec3 &lightColorVec)
{
	const int maxColourVal = 255;
	const int minColourVal = 1;

	//Cycle 
	if (lightColorVec.z == maxColourVal && lightColorVec.x != maxColourVal && lightColorVec.y == minColourVal)
		lightColorVec.x++;
	else if (lightColorVec.x == maxColourVal && lightColorVec.z != minColourVal)
		lightColorVec.z--;
	else if (lightColorVec.x == maxColourVal && lightColorVec.y != maxColourVal)
		lightColorVec.y++;
	else if (lightColorVec.y == maxColourVal && lightColorVec.x != minColourVal)
		lightColorVec.x--;
	else if (lightColorVec.y == maxColourVal && lightColorVec.z != maxColourVal)
		lightColorVec.z++;
	else if(lightColorVec.z == maxColourVal && lightColorVec.y != minColourVal)
		lightColorVec.y--;
	else
		lightColorVec.z = maxColourVal;

	// Change based off input 
	/*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && lightColorVec.x < maxColourVal)
		lightColorVec.x++;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && lightColorVec.y < maxColourVal)
		lightColorVec.y++;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && lightColorVec.z < maxColourVal)
		lightColorVec.z++;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightColorVec.x = minColourVal;
		lightColorVec.y = minColourVal;
		lightColorVec.z = minColourVal;
	}*/
}


void wireFrame(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void initialize(int xPos, int yPos)
{
	try
	{
		if (!glfwInit()){ throw std::string("Failed to initialize GLFW"); }

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(xPos, yPos, "Playground", NULL, NULL);
		if (window == NULL) { throw std::string("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible."); }
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) { throw std::string("Failed to initialize GLEW"); }

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		//Hide mouse 
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
	}
	catch(std::string e)
	{
		std::cout << e << std::endl;
	}
}
