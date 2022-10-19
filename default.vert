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