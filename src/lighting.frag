#version 330 core
in vec3 vertexNormal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 color;

void main()
{
	float ambiantStrength = 0.1;
	vec3 ambiantColor = ambiantStrength * lightColor;
	
	vec3 norm = normalize(vertexNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 diffuseColor = max(dot(norm, lightDir), 0.0) * lightColor;
	
    FragColor = vec4((ambiantColor + diffuseColor) * color, 1.0);
}
