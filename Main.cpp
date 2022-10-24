#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

#include "SurfaceSquare.h"
#include "SurfacePentagon.h"
#include "FaceData.h"
#include "FaceDataGenerator.h"

const unsigned int width = 1300;
const unsigned int height = 800;

GLfloat zoom = 0.5f;
GLfloat x_vel = 0.01f, y_vel = 0.008f;
GLfloat x_pos = 0.0f, y_pos = 0.0f;
GLfloat x_max = width / (zoom * 700.0f), y_max = height / (zoom * 700.0f);

const GLfloat spread = 2.0f; // how extreme we interpolate
							 // 1.0f corresponds to original 




int main() {
	glfwInit();

	// tell glfw what version of openGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "FlowerDodec", NULL, NULL);
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
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	// VBO - vertex buffer object: stores data in GPU memory 
	// VAO - vertex array object: stores pointers to one or more VBOs
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// SurfaceSquare surface(-1.0f, -1.0f, 1.0f, 1.0f, 50, 50);
	SurfacePentagon surface(4);
	surface.generate();
	FaceDataGenerator facedatgen;
	std::vector<FaceData> dodecFaceData = facedatgen.genSimpleData();


	// TODO update VBO to work with vectors
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&surface.vertices[0], surface.vertices.size() * sizeof(GLfloat));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&surface.indices[0], surface.indices.size() * sizeof(GLuint));

	// Links VBO to VAO
	// link attributes for coordinates
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0 );
	// link attribute for the morph targets
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	float rotation = 0.0f;
	GLfloat prevTime = glfwGetTime();

	// generate the data about the cube faces



	glEnable(GL_DEPTH_TEST);

	// initialise the matrices we'll need
	glm::mat4 model = glm::mat4(1.0f); // leave as is since model in the centre
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 ortn = glm::mat4(1.0f); // orientation matrix

	// move the world down 0.0 and forward 4.0
	glm::vec3 camPos(0.0f, 0.0f, 4.0f);
	view = glm::translate(view, -camPos);
	view = glm::scale(view, glm::vec3(zoom, zoom, zoom));
	// set our camera,		FOV					aspect ratio       		closest furthest we can see
	//proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// lighting
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 5.0f); // relative to shape

	// loop until the window is closed
	while (!glfwWindowShouldClose(window)) {
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		GLfloat crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1.0f / 60.0f)
		{
			rotation += 0.5f;
			x_pos += x_vel;
			y_pos += y_vel;
			if (glm::abs(x_pos) > x_max)
				x_vel *= -1.0f;
			if (glm::abs(y_pos) > y_max)
				y_vel *= -1.0f;
			prevTime = crntTime;
			
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(x_pos, y_pos, 0.0f));
			model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 1.0f, 1.0f));

			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

			int ortnLoc = glGetUniformLocation(shaderProgram.ID, "ortn");
			glUniformMatrix4fv(ortnLoc, 1, GL_FALSE, glm::value_ptr(ortn));

			int transLoc = glGetUniformLocation(shaderProgram.ID, "trans");
			glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

			int morphLoc = glGetUniformLocation(shaderProgram.ID, "morph");
			glUniform1f(morphLoc, spread * (4.5 - std::abs(1.6 * std::fmod(prevTime / spread, 7.5f) - 6)));
			// glUniform1f(morphLoc, 0.0f); // no morph
			int colorLoc = glGetUniformLocation(shaderProgram.ID, "color");

			glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
				lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),
				lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"),
				camPos.x, camPos.y, camPos.z);



			// Bind the VAO so OpenGL knows to use it
			VAO1.Bind();
			// Draw primitives, number of indices, datatype of indices, index of indices
			// generalise so we don't need to keep track of the number of indices
			for (FaceData face : dodecFaceData) {
				glUniformMatrix4fv(ortnLoc, 1, GL_FALSE, glm::value_ptr(face.orientation));
				glUniform3f(colorLoc, face.color[0], face.color[1], face.color[2]);
				glDrawElements(GL_TRIANGLES, surface.indices.size(), GL_UNSIGNED_INT, 0);
			}

			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);

		}
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}