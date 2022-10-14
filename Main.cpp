#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

// vertices for a triangle
GLfloat vertices[] = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// indices for vertex order - so we can reuse vertices
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

int main() {
	glfwInit();

	// tell glfw what version of openGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(800, 800, "FlowerDodec", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// now tell glfw to use the window we just created
	glfwMakeContextCurrent(window);

	// have glad load the needed configurations for openGl
	gladLoadGL();

	// tell openGL the area of the window we want it to render in
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	
	// VBO - vertex buffer object: stores data in GPU memory 
	// VAO - vertex array object: stores pointers to one or more VBOs
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO); // must generate VAO before VBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// store our vertices in the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// make our element array current, aka - bind it	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// then introduce the indces
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// configure the VAO so openGL knows how to read the VBOs it stores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind both the VAO and VBO so a function can't accidentally change them 
	glBindBuffer(GL_ARRAY_BUFFER, 0); // bind to zero not what we had before
	glBindVertexArray(0); // order matters 
	// Bind the EBO to 0 so that we don't accidentally modify it
	// MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
	// This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// loop until the window is closed
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}