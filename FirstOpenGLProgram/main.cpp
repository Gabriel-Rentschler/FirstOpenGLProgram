#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "main.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec4 customColor;\n"
"out vec4 RGBA;\n"
"void main() {\n"
"	RGBA = customColor;\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void shaderCompile(unsigned int shader, const char* shaderSource, const char* message) {
	int success;
	char infoLog[512];

	//Attach the shader source code to the object and compile it
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//Check if compiled succesfully
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << message <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
	//Checks if window is not created, print message, terminate glfw.
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	//set the function we created to be called on every window resize.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD, with GLFW passing the address of the OpenGL functions for it to load
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//\/\/\/\/\/\/\/\/\/\//
	//      SHADERS      //
	//\/\/\/\/\/\/\/\/\/\//

	//Create and compile Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shaderCompile(vertexShader, vertexShaderSource, "VERTEX");

	//Create and compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderCompile(fragmentShader, fragmentShaderSource, "FRAGMENT");

	//SHADER PROGRAM
	unsigned int shaderProgram = glCreateProgram();

	//Attach the shaders into the program and run the linker
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for compile errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED" << infoLog << std::endl;
	}

	//Delete the shaders, since it is already linked to the program they won't be used anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//\/\/\/\/\/\/\/\/\/\//
	//    VERTEX DATA    //
	//\/\/\/\/\/\/\/\/\/\//

	//Create the Normalized Device Coordinates NDC
	float vertices[] {
		-0.5f, -0.5f, 0.0f, //bottom left
		0.5f, -0.5f, 0.0f, //bottom right
		0.0f, 0.5f, 0.0f //middle top
	};

	//Create a EBO for a rectangle
	float recVertices[]{
		0.7f, 0.7f, 0.0f, //top right
		0.7f, 0.5f, 0.0f, //bottom right
		0.5f, 0.5f, 0.0f, //bottom left
		0.5f, 0.7f, 0.0f //top left
	};
	unsigned int indices[]{
		0, 1, 3,
		1, 2, 3
	};

	//Create VAO (Vertex Array Object)
	unsigned int VAO[2], VBO[2];

	//Create EBO (Element Buffer Object)
	unsigned int EBO;

	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);

	glGenBuffers(2, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Bind the VAO | All VBOs will be stored here from now on
	glBindVertexArray(VAO[1]);

	//Bind the buffer object to the ARRAY_BUFFER type, which is a VBO type
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	//Copy the vertex data to GPU memory | If the data was to change a lot of times, use GL_DYNAMIC_DRAW instead
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Show OpenGL how to interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

		// Get the green value in a sin so it gradually changes
		float timeValue = glfwGetTime();
		float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");

		//Draw the object
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, colorValue, colorValue, 0.0f, 1.0f);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO[1]);
		//Draw triangle primitives, starting at index 0 on the VAO, using 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Call Events and Buffer Swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}