#version 330 core
out vec4 FragColor;

// R�cup�re la couleur depuis le Vertex Shader
in vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}