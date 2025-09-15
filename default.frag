#version 330 core

// Renvoi la couleur en RGBA
out vec4 FragColor;

// Injecte la couleur depuis le Vertex Shader
in vec3 color;
// Injecte l'UV de la texture depuis le Vertex Shader
in vec2 texCoord;

// Récupère la Texture Unit depuis la fonction main
uniform sampler2D tex0;

// Récupère la couleur de la source de lumière
uniform vec4 lightColor;

void main()
{
	// Je multiplie la couleur du pixel la couleur de la source de lumière
	FragColor = texture(tex0, texCoord) * lightColor;
}