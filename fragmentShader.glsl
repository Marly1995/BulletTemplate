#version 330
in vec4 fragmentColor;
out vec4 outputColor;

uniform vec4 color;
uniform vec3 lightColor;
void main()
{
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * lightColor;
	
	outputColor = color * vec4(ambient, 1.0f);
}
