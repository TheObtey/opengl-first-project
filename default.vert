#version 330 core

// Coordonnées
layout (location = 0) in vec3 aPos;
// Couleur
layout (location = 1) in vec3 aColor;
// UV (Coordonnées de texture)
layout (location = 2) in vec2 aTex;

// Renvois la couleur pour le Fragment Shader
out vec3 color;

// Renvois l'UV de la texture
out vec2 texCoord;

// J'importe la matrice de la caméra depuis la fonction main
uniform mat4 camMatrix;

void main()
{
	// Renvois la position/coordonnée de toutes les vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);

	// Assigne de la couleur des Vertex Data à "color"
	color = aColor;
	// Assigne les coordonnées de texture des Vertex Data à "texCoord"
	texCoord = aTex;
}