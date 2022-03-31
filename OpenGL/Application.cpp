#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error!!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = 
	{
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom left
		 0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // top 
	};
	unsigned int indices[] = 
	{  
		// note that we start from 0!
		0, 1, 2,   // first triangle
	};

	unsigned int VAO;
	glCreateVertexArrays(1, &VAO);
	// bind Vertex Array Object
	glBindVertexArray(VAO);
	
	unsigned int VBO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("Shaders/Shader.vs.txt", "Shaders/Shader.fs.txt");

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{ 
		/* Render here */ 
		glClear(GL_COLOR_BUFFER_BIT);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO);
		shader.use();

// 		float timeValue = glfwGetTime();
// 		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
// 		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
// 		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents(); 
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
/*	glDeleteProgram(shaderProgram);*/

	glfwTerminate();
	return 0;
}