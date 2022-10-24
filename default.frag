#version 330 core
out vec4 FragColor;
in vec3 vertcolor;
in vec4 crntPos;
in vec4 normal;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
   float ambient = 0.15f;

   vec3 nnorm = normalize(vec3(normal));
   vec3 lightDirection = normalize(lightPos - vec3(crntPos));
   float diffuse = max(dot(nnorm, lightDirection), 0.0f);

   // specular lighting
   float specularLight = 0.33f;
   vec3 viewDirection = normalize(camPos - vec3(crntPos));
   vec3 reflectionDirection = reflect(-lightDirection, nnorm);
   float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 75);
   float specular = specAmount * specularLight;

   FragColor = vec4(vertcolor, 1.0f) * lightColor * (ambient + diffuse*(1.0f - ambient - specularLight) + specular);
   //FragColor = vec4(vec3(crntPos), 1.0f);
}