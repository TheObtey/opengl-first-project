#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// D�finis le type de texture de l'objet 'Texture'
	type = texType;

	// D�clare la taille et le nombre de canneaux de couleur de l'image
	int widthImg, heightImg, numColCh;
	// Retourne l'image verticallement
	stbi_set_flip_vertically_on_load(true);
	// Lis l'image et stock le r�sultat sous forme de bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);

	// G�n�re un objet "OpenGL Texture"
	glGenTextures(1, &ID);
	// Assigne la texture � un Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configure le typer d'algo utilis� pour rendre l'image plus ou moins grosse (ici c'est NEAREST)
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure la fa�on dont la texture se r�p�te
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Assigne l'image � l'objet "OpenGL Texture"
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	// Supprime les datas de l'image puisqu'elle sont maintenant stock� dans l'objet "OpenGL Texture"
	stbi_image_free(bytes);

	// Unbind l'objet "OpenGL Texture" pour qu'il ne puisse pas �tre accidentellement modifi�
	glBindTexture(texType, 0);
};

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Le shader doit �tre activ� avant de changer la valeur d'un uniform
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