#version 330 core
out vec4 FragColor;

// Injecte la couleur depuis le Vertex Shader
in vec3 color;
// Injecte l'UV de la texture depuis le Vertex Shader
in vec2 texCoord;

// Récupère la Texture Unit depuis la fonction main
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}