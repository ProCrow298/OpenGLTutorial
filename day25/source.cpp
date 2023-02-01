#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imageLoader/stb_image.h>
#include <shaders/shader.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
glm::mat4 DVDtransform(glm::mat4 transform);
void randomColorChange();

const unsigned int SCR_WIDTH = 500;
const unsigned int SCR_HEIGHT = 500;
glm::vec2 velocity(0.5f, 0.5f);
glm::vec2 position((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DVD?", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader shader("vertexShader.vert", "fragmentShader.frag");

    float vertices[] = {
        // positions          // texture coords
         0.25f,  0.25f, 0.0f,   1.0f, 1.0f, // top right
         0.25f, -0.25f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.25f, -0.25f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.25f,  0.25f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, 
        1, 2, 3  
    };

    unsigned int VBO, VAO, EBO;
    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("OpenGL.png", &width, &height, &nrChannels, 0); 
 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    stbi_image_free(data);

    shader.use();

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = DVDtransform(transform);
        //transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // render container
        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
       

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::mat4 DVDtransform(glm::mat4 transform)
{
    if (position.x >= .75)
    {
        velocity.x = -.5;
        randomColorChange();
    }
    else if (position.x <= -.75)
    {
        velocity.x = .5;
        randomColorChange();
    }
    if (position.y >= .90)
    {
        velocity.y = -.5;
        randomColorChange();
    }
    else if (position.y <= -.90)
    {
        velocity.y = .5;
        randomColorChange();
    }
    
    position.x += velocity.x * 0.002;
    position.y += velocity.y * 0.002;
        
    transform = glm::translate(transform, glm::vec3(position, 0));
 
    return transform;
}

void randomColorChange()
{
    Shader shader("vertexShader.vert", "fragmentShader.frag");

    int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
    glUniform4f(vertexColorLocation,
        (double)rand() / (double)RAND_MAX,
        (double)rand() / (double)RAND_MAX,
        (double)rand() / (double)RAND_MAX, 1.0f);
}