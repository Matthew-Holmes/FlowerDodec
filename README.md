# FlowerDodec

A recreation of the classic microsoft flowerbox screensaver in OpenGL - except now dodecahedral. 

## Process

### 1. Template

The template for this project is that used during the OpenGL course: https://www.youtube.com/watch?v=45MIykWJ-C4 at first following the first 7 chapters until "Going 3D" https://github.com/VictorGordan/opengl-tutorials. This provides the necessary file structures, shaders and drawing loop to build upon.

### 2. Recreating the original

Recreating the original provided both an understanding of how the geometry was generated, what sort of classes to use to achieve this, and how to exploit the symmetry of the polyhedra to optimise the efficiency of the code. I used similar methods to a previous java implementation found at https://github.com/kevin-shannon/3D-FlowerBox. However I outsourced more work to the shaders to avoid having to refill the Vertex Buffer Object used (VBO).

#### Custom vertex shader

To both exploit symmetries and use the same VBO I edited the vertex shader used in the tutorial. Now to take two coordinate vectors per vertex and return an interpolation between them. Also applying an orientation matrix so that the same geometry can be rotated in space to form each side of the cube. since all vertices in a face have the same color, I made this a uniform variable instead of stored for each vertex in the VBO.

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
