#version 330
in vec3 position;
in vec3 normal;
in vec4 vertexColor;
out vec4 FragmentColor;
out vec3 FragmentPos;
out vec3 Normal;

uniform mat4 modelMatrix      = mat4(1.0);
uniform mat4 viewMatrix       = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
		FragmentColor = vertexColor;
		
		Normal = normal;
		FragmentPos = vec3(modelMatrix * vec4(position, 1.0));
		
}
