#ifndef SURFACEPENTAGON_CLASS_H
#define SURFACEPENTAGON_CLASS_H

#include<glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

class SurfacePentagon {
public:
	SurfacePentagon(GLuint meshd);

	void generate();
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
private:;
	void gen_seg();
	void gen_seg_outline();

	static const GLfloat A;
	static const GLfloat R;
	const GLuint mesh_depth;
};
#endif