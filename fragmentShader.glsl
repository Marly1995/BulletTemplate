#version 330
in vec4 fragmentColor;
in vec3 FragmentPos;

in vec3 Normal;
out vec4 outputColor;

uniform vec4 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
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
	
	// specular
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(cameraPos - FragmentPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 lighting = ambient + diffuse + specular;
	outputColor = color * vec4(lighting, 1.0);
}
