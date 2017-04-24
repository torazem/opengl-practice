#include "stdafx.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

int main()
{
	if (!glfwInit())
	{
		std::cout << "FAIL: glfw not initialised";
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(800, 600, "WHOOHOO!", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);

	if (!window)
	{
		std::cout << "FAIL: window not created";
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	std::cout << "GLEW Errors: " << err << "\n";

	double time = glfwGetTime();
	glfwSwapInterval(1);

	float vertices[] = {
		-0.0f,  0.5f, // Vertex 1 (X, Y)
		0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint vertexShaderStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint fragmentShaderStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint positiveAttribute = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(positiveAttribute);
	glVertexAttribPointer(positiveAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	std::cout << "Vertex Shader Compiled: " << vertexShaderStatus << "\n";
	std::cout << "Fragment Shader Compiled: " << fragmentShaderStatus << "\n";
	std::cout << "Error Code: " << glGetError() << "\n";

	while (!glfwWindowShouldClose(window))
	{
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, width, height);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Error Code: " << glGetError() << "\n";

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
	exit(EXIT_SUCCESS);
}