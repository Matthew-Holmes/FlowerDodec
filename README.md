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

The "flower" effect is a result of interpolating each point with its normalised (in length) counterpart, with the centre of the square at (x, y, z) of (0, 0, 1), hence the circular symmetry around the centre. The spiked protrusions later in the animation loop are a result of the interpolation value `morph` taking larger values than in a standard interpolation. (greater in magnitude than 1)

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

#### Storing face data

All that remains is to keep track of the orientations of each of the six faces and their colour, a data class `FaceData` stores this:
```
class FaceData {
public:
	FaceData(glm::mat4 ort, glm::vec3 col) : orientation(ort), color(col) {};
	glm::mat4 orientation;
	glm::vec3 color;
};
```
And so we can fill a vector with data for each of the six faces:
```
std::vector<FaceData> cubeFaceData;
cubeFaceData.emplace_back(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
cubeFaceData.emplace_back(
	glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
	glm::vec3(0.0f, 1.0f, 0.0f));
	// orientation matrix
	// then color
// four more faces to fill
```

#### Drawing the faces
After making sure the time uniform variable has been updated, and our buffers are in order, we can draw the shape on each update using the following loop:
```
for (FaceData face : cubeFaceData) {
	glUniformMatrix4fv(ortnLoc, 1, GL_FALSE, glm::value_ptr(face.orientation)); // update orientation uniform for vertex shader
	glUniform3f(colorLoc, face.color[0], face.color[1], face.color[2]);         // update color uniform for vertex shader
	glDrawElements(GL_TRIANGLES, square.indices.size(), GL_UNSIGNED_INT, 0);    // draw
}
```

#### Movement

A simple bouncing movement was added, sending the position of the shape to the shaders as a uniform, which will be required in the next step.

### 2. lighting
The current shape is now geometrically accurate, but wihthout lighting looks flat. The next step was adding diffuse, specular and ambient light in the right proportions, as shown in chapter 9 of https://www.youtube.com/watch?v=45MIykWJ-C4. This required access to normal vectors - unfortunately since the shaders compute individual vertex positions ad hoc this is not so simple.

#### interpolating normals

The geometry calls for interpolating between points on a cube's face, and their normalised counterparts - which we observe lie on the surface of the unit sphere. Thankfully both shapes have trivial normals so we opt to interpolate these too - the mathematically legitmacy of such an operation is questionable, however the res0ults appear realistic.

The vertex shader computes:
```
normal = model * ortn * vec4(vec3(0.0, 0.0, 1.0) * (1.0 - morph) + bPos * morph, 1.0);
```
Where `(0.0, 0.0, 1.0)` is normal to our default square that lies flat in the X-Y plane, and the `bPos` already is the normal, as well as the position - since lies on the unit sphere. We then pass this onto the fragment shader which computes:

```
float ambient = 0.1f;

vec3 nnorm = normalize(vec3(normal));
vec3 lightDirection = normalize(lightPos - vec3(crntPos));
float diffuse = max(dot(nnorm, lightDirection), 0.0f);

// specular lighting
float specularLight = 0.33f;
vec3 viewDirection = normalize(camPos - vec3(crntPos));
vec3 reflectionDirection = reflect(-lightDirection, nnorm);
float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 50);
float specular = specAmount * specularLight;

FragColor = vec4(vertcolor, 1.0f) * lightColor * (ambient + diffuse*(1.0f - ambient - specularLight) + specular);
```

The result gives our shape a shiny appearance and the appearance of moving against a dark backdrop. We store the apparent position of the viewer relative to the shapes coordinate system in `camPos`.

#### note on performance

Here we have opted to trade memory performance for computational - since the animation loop is only 7.5 seconds we could simply compute the geometry for each frame in advance, then compute the normals using traditional methods, and avoid any on the fly calculations. However the given approach was taken since memory reads a more intensive task than flops and this approach allows for the same VBO to be preserved for the entire program. Also for finer meshes this approximate 100x memory allocation could prove the limiting factor.

An alternative would be to compute the geometry and normals each step to fill a buffer, keeping memory demands low, however this effectively already accomplished by the shaders and so would only be preferable if we believed the shader programs could not perform the task as effiently as the c++ code.

