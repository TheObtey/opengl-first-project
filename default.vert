#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Renvois la couleur pour le Fragment Shader
out vec3 color;

uniform float scale;

void main()
{
	gl_Position = vec4(
		aPos.x + aPos.x * scale,
		aPos.y + aPos.y * scale,
		aPos.z + aPos.z * scale,
		1.0
	);
	// Assigne de la couleur des Vertex Data à "color"
	color = aColor;
}