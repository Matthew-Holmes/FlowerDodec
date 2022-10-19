# FlowerDodec

A recreation of the classic microsoft flowerbox screensaver in OpenGL - except now with a dodecahedron.

## Process

### 1. Template

The template for this project is that used during the OpenGL course: https://www.youtube.com/watch?v=45MIykWJ-C4 following the first 7 chapters until "Going 3D" https://github.com/VictorGordan/opengl-tutorials. This provides the necessary file structures, shaders and drawing loop to build upon.

### 2. Recreating the original

Recreating the original provided both an understanding of how the geometry was generated, what sort of classes to use to achieve this, and how to exploit the symmetry of the polyhedra to optimise the efficiency of the code. I used similar methods to a previous java implementation found at https://github.com/kevin-shannon/3D-FlowerBox. However I outsourced more work to the vetex shader to avoid having to refill the Vertex Buffer Object used (VBO).

#### Custom vertex shader

To both exploit symmetries and use the same VBO I edited the vertex shader used in the tutorial. Now it take two coordinate vectors per vertex and returns an interpolation between them. Also applying an orientation matrix so that the same geometry can be rotated in space to form each side of the cube - which allows for the same single face to be drawn at all sides of the polyhedron. since all vertices in a face have the same color, I made this a uniform variable instead of stored for each vertex in the VBO.

```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 bPos;

out vec3 vertcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 ortn;
uniform float morph;
uniform vec3 color;


void main()
{
   gl_Position = proj * view * model * ortn * vec4(aPos * (1.0 - morph) + bPos * morph, 1.0);
   vertcolor = color;
}
```

The "flower" effect is a result of interpolating each point with its normalised (in length) counterpart, with the centre of the square at (x, y, z) of (0, 0, 1), hence the circular symmetry around the centre. The spiked protrusions later in the animation loop are a result of the interpolation value `morph` taking larger values than in a standard interpolation. (greater than 1)

The VBO stores the vertices on the surface of the face, and their normalised counterparts one after the other in the VBO. This means the VBO can persist for the lifetime of the program, just changing the time variable at each update. Since the `morph` value is a function of time.

#### The `SurfaceSquare` class

To compute the necessary mesh coordinates and converte them to vertices and indices for the VBO a class `SurfaceSquare` was created, declared as follows:
```
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
```
Once provided with initial coordinates for the square, and a granularity for the mesh divisions, we can called `generate()` to fill vectors with the vertices and indices required, and then fill the buffers:
```
	SurfaceSquare square(-1.0f, -1.0f, 1.0f, 1.0f, 50, 50);
	square.generate();
   // Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&square.vertices[0], square.vertices.size() * sizeof(GLfloat));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&square.indices[0], square.indices.size() * sizeof(GLuint));
```
