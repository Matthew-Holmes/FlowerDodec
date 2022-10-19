#include "SurfaceSquare.h"


SurfaceSquare::SurfaceSquare(GLfloat xi, GLfloat yi, GLfloat xf, GLfloat yf,
							 GLuint xdv, GLuint ydv) :
	x_initial(xi), y_initial(yi), x_final(xf), y_final(yf),
	x_div(xdv), y_div(ydv), 
	numVert((xdv + 1) * (ydv + 1)), numInd(numVert * 6) {}

void SurfaceSquare::genMesh() {
	for (GLuint i = 0; i <= x_div; i++) {
		x_intervals.push_back(x_initial + i * ((x_final - x_initial) / x_div));
	}
	for (GLuint j = 0; j <= y_div; j++) {
		y_intervals.push_back(y_initial + j * ((y_final - y_initial) / y_div));
	}
}

void SurfaceSquare::genVrts() {
	GLfloat norm;
	for (auto x : x_intervals) {
		for (auto y : y_intervals) {
			norm = std::sqrt(x * x + y * y + 1);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(1);
			vertices.push_back(x / norm);
			vertices.push_back(y / norm);
			vertices.push_back(1.0f / norm);
		}
	}
}

void SurfaceSquare::genInds() {
	// loop over the squares in the grid
	for (GLuint i = 0; i < x_div; i++) {
		for (GLuint j = 0; j < y_div; j++) {
			// index of top left vertex in vertices array
			// treating the array as an array of triples
			GLuint n = j * (x_div + 1) + i;
			indices.push_back(n); indices.push_back(n + 1); indices.push_back(n + x_div + 1);
			indices.push_back(n + 1); indices.push_back(n + x_div + 1); indices.push_back(n + x_div + 2);
		}
	}
}

void SurfaceSquare::generate() {
	genMesh();
	genVrts();
	genInds();
}
