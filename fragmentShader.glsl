#version 330
in vec4 fragmentColor;
in vec3 FragmentPos;

in vec3 Normal;
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
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragmentPos);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 lighting = ambient + diffuse;
	outputColor = color * vec4(lighting, 1.0);
}
