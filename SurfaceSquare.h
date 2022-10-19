#ifndef SURFACESQUARE_CLASS_H
#define SURFACESQUARE_CLASS_H

#include<glad/glad.h>
#include <vector>
#include <cmath>

class SurfaceSquare {
public:
	SurfaceSquare(GLfloat xi, GLfloat yi, GLfloat xf, GLfloat yf,
		GLuint xdv, GLuint ydv);
	void generate();
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;	
private:
	void genMesh();
	void genVrts();
	void genInds();

	const GLfloat x_initial, y_initial, x_final, y_final;
	const GLuint x_div, y_div;
	const GLuint numVert, numInd;
	std::vector<GLfloat> x_intervals, y_intervals; // mesh variables

};
#endif