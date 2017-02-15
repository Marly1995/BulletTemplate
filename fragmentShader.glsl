#version 330
in vec4 fragmentColor;
out vec4 outputColor;

uniform vec4 color;
void main()
{
	 outputColor = color;
}
