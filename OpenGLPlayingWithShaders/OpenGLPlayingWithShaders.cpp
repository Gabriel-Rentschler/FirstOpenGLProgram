#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Shader.h"
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main() {\n"
"gl_Position = vec4(aPos, 1.0);\n"
"vertexColor = color;\n"
"}\0";
const char* fragmentShaderOrangeSource = "#version 330 core\n"
"uniform vec4 customColor;\n"
//"in vec3 vertexColor;\n"
"out vec4 RGBA;\n"
"void main() {\n"
"	RGBA = customColor;\n"
"}\0";
const char* fragmentShaderYellowSource = "#version 330 core\n"
"out vec4 RGBA;\n"
"in vec3 vertexColor;\n"
"void main() {\n"
"	RGBA = vec4(vertexColor, 1.0f);\n"
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
		std::cout << "ERROR::SHADER::" << message << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void shaderProgramCompile(unsigned int shaderProgram) {
	glLinkProgram(shaderProgram);

	//Check for compile errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED" << infoLog << std::endl;
	}
}


int main()
{
	///////////
	// SETUP //
	///////////
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	Shader firstShader("./VertexShader.txt", "./FragmentShader.txt");

	//\/\/\/\/\/\/\/\/\/\//
	//    VERTEX DATA    //
	//\/\/\/\/\/\/\/\/\/\//
	float v_triangle1[]{
		-1.0f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, //bottom left
		0.0f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, //bottom right
		-0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 1.0f //middle top
	};
	float v_triangle2[]{
		//Positions				//Colors
		0.0f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, //bottom left
		1.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, //bottom right
		0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f //middle top
	};
	//Create VAO (Vertex Array Object)
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	//TRIANGLE 1
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v_triangle1), v_triangle1, GL_STATIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//glBindVertexArray(0); //No need to bind since we are setting up another VAO
	//TRIANGLE 2
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v_triangle2), v_triangle2, GL_STATIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/////////////////
	// RENDER LOOP //
	/////////////////
	while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		firstShader.use();
		//firstShader.setFloat("someUniform", 1.0f);
		glBindVertexArray(VAO[1]);
		//Draw triangle primitives, starting at index 0 on the VAO, using 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Call Events and Buffer Swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}