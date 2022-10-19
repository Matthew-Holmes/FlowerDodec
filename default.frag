#version 330 core
out vec4 FragColor;
in vec3 vertcolor;

uniform vec4 lightColor;
uniform vec3 lightPos;

void main()
{
   FragColor = vec4(vertcolor, 1.0f) * lightColor;
}