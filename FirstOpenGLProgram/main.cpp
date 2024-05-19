#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "main.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//glfw initializes, we pass the minor and major version we want of OpenGL to run, which is version 3
	//we also set the core profile as the default
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

	//Creates window, 800x600 and sets the title
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);

	//set the function we created to be called on every window resize.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Checks if window is not created, print message, terminate glfw.
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD, with GLFW passing the address of the OpenGL functions for it to load
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//GLAD tells OpenGL size of the rendering window to do math right and display with respect to the window size.
	//First two params set the location of the lower left corner of the window, the other two set width and height.
	glViewport(0, 0, 800, 600);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);

		/////////////
		//Rendering//
		/////////////
		//Clear the screen to a specific color (the example is a green)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Call Events and Buffer Swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}