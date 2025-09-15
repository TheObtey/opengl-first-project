#version 330 core

// Coordonn�es
layout (location = 0) in vec3 aPos;
// Couleur
layout (location = 1) in vec3 aColor;
// UV (Coordonn�es de texture)
layout (location = 2) in vec2 aTex;

// Renvois la couleur pour le Fragment Shader
out vec3 color;

// Renvois l'UV de la texture
out vec2 texCoord;

// J'importe la matrice de la cam�ra depuis la fonction main
uniform mat4 camMatrix;

void main()
{
	// Renvois la position/coordonn�e de toutes les vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);

	// Assigne de la couleur des Vertex Data � "color"
	color = aColor;
	// Assigne les coordonn�es de texture des Vertex Data � "texCoord"
	texCoord = aTex;
}