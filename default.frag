#version 330 core

// Renvoi la couleur en RGBA
out vec4 FragColor;

// Importe la couleur depuis le Vertex Shader
in vec3 color;
// Importe l'UV de la texture depuis le Vertex Shader
in vec2 texCoord;
// Importe le Normal depuis le Vertex Shader
in vec3 Normal;
// Importe la position actuelle depuis le Vertex Shader
in vec3 crntPos;

// R�cup�re la Texture Unit depuis la fonction main
uniform sampler2D tex0;

// R�cup�re la couleur de la source de lumi�re
uniform vec4 lightColor;
// R�cup�re la position de la source de lumi�re
uniform vec3 lightPos;
// R�cup�re la position de la cam�ra
uniform vec3 camPos;

void main()
{
	// Lumi�re ambiante
	float ambiant = 0.20f;

	// Lumi�re diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Lumi�re sp�culaire
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// Je multiplie la couleur du pixel par la couleur de la source de lumi�re
	// puis par la somme de la lumi�re ambiante + lumi�re diffuse + sp�culaire
	FragColor = texture(tex0, texCoord) * lightColor * (ambiant + diffuse + specular);
}