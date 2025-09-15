#version 330 core

// Coordonnées
layout (location = 0) in vec3 aPos;
// Couleur
layout (location = 1) in vec3 aColor;
// UV (Coordonnées de texture)
layout (location = 2) in vec2 aTex;
// Normale (pas nécessairement normalisé)
layout (location = 3) in vec3 aNormal;

// Renvois la couleur pour le Fragment Shader
out vec3 color;

// Renvois l'UV de la texture pour le Fragment Shader
out vec2 texCoord;

// Renvois la Normale pour le Fragment Shader
out vec3 Normal;
out vec3 crntPos;

// J'importe la matrice de la caméra depuis la fonction main
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));

	// Renvois la position/coordonnée de toutes les vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);

	// Assigne de la couleur des Vertex Data à "color"
	color = aColor;
	// Assigne les coordonnées de texture des Vertex Data à "texCoord"
	texCoord = aTex;
	Normal = aNormal;
}