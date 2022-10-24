#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 bPos;

out vec3 vertcolor;
out vec4 normal;
out vec4 crntPos;

uniform mat4 model;
uniform mat4 trans;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 ortn;
uniform float morph;
uniform vec3 color;


void main()
{
   vec4 inter = vec4(aPos * (1.0 - morph) + bPos * morph, 1.0);
   crntPos =  trans * model * ortn * inter;
   gl_Position = proj * view * crntPos;
   normal = model * ortn * vec4(vec3(0.0, 0.0, 1.0) * (1.0 - morph) + bPos * morph, 1.0)
          * ( 1 - 2 * int (inter.z < 0.0) );
   vertcolor = color;
}