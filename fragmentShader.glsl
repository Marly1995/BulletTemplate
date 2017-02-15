#version 330
in vec4 fragmentColor;
in vec3 FragmentPos;
out vec4 outputColor;

uniform vec4 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
void main()
{
	// ambient 
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	
	// diffuse
	vec3 lightDirection = normalize(lightPos - FragmentPos);
	vec3 diffuse = lightDirection * lightColor;
	
	vec3 lighting = ambient + diffuse;
	outputColor = color * vec4(lighting, 1.0);
}
