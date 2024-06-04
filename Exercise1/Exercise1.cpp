#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 RGBA;\n"
"void main() {\n"
"	RGBA = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	float vertices[]{
		//TRIANGLE 1
		-1.0f, -0.5f, 0.0f, //bottom left
		0.0f, -0.5f, 0.0f, //bottom right
		-0.5f, 0.5f, 0.0f, //middle top
		//TRIANGLE 2
		0.0f, -0.5f, 0.0f, //bottom left
		1.0f, -0.5f, 0.0f, //bottom right
		0.5f, 0.5f, 0.0f, //middle top
	};

	//Create VAO (Vertex Array Object)
	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);

	//Bind the VAO | All VBOs will be stored here from now on
	glBindVertexArray(VAO);

	//Create the Vertex Buffer Object VBO
	glGenBuffers(1, &VBO);

	//Bind the buffer object to the ARRAY_BUFFER type, which is a VBO type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copy the vertex data to GPU memory | If the data was to change a lot of times, use GL_DYNAMIC_DRAW instead
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Show OpenGL how to interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		//Draw the object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//Draw triangle primitives, starting at index 0 on the VAO, using 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Call Events and Buffer Swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
