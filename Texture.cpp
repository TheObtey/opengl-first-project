#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Définis le type de texture de l'objet 'Texture'
	type = texType;

	// Déclare la taille et le nombre de canneaux de couleur de l'image
	int widthImg, heightImg, numColCh;
	// Retourne l'image verticallement
	stbi_set_flip_vertically_on_load(true);
	// Lis l'image et stock le résultat sous forme de bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);

	// Génère un objet "OpenGL Texture"
	glGenTextures(1, &ID);
	// Assigne la texture à un Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configure le typer d'algo utilisé pour rendre l'image plus ou moins grosse (ici c'est NEAREST)
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure la façon dont la texture se répète
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assigne l'image à l'objet "OpenGL Texture"
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	// Supprime les datas de l'image puisqu'elle sont maintenant stocké dans l'objet "OpenGL Texture"
	stbi_image_free(bytes);

	// Unbind l'objet "OpenGL Texture" pour qu'il ne puisse pas être accidentellement modifié
	glBindTexture(texType, 0);
};

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Le shader doit être activé avant de changer la valeur d'un uniform
	shader.Activate();
	glUniform1i(texUni, unit);
};

void Texture::Bind()
{
	glBindTexture(type, ID);
};

void Texture::Unbind()
{
	glBindTexture(type, 0);
};

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}