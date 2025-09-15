#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;

// D�claration des coordon�es des vertices
GLfloat vertices[] =
{ //	COORDONNEES		/		COULEURS		/		UV
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Coin inf�rieur droit
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, // Coin sup�rieur droit
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Coin supp�rieur gauche
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, // Coin inf�rieur gauche
};

// D�claration du tableau d'indices, il permet de d�finir l'ordre dans lequel les sommets doivent �tre rendu.
GLuint indices[] =
{
	0, 2, 1, // Triangle sup�rieur
	0, 3, 2 // Triangle inf�rieur
};

int main()
{
	// J'initialise la librarie GLFW
	// C'est un genre d'API pour utiliser OpenGL
	glfwInit();

	// Je pr�cise � GLFW quelle version d'OpenGL j'utlise
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Je pr�cise � GLFW que j'utilise le CORE profile
	// En gros c'est pour dire qu'on utilisera que les fonctions r�centes
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Je cr�e une instance de l'objet GLFWwindow pour cr�er ma fen�tre
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

	// Je v�rifie que la fen�tre est bien cr��
	if (!window)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Je dis � OpenGL qu'il doit push le rendu sur mon objet 'window'
	glfwMakeContextCurrent(window);

	// J'initialise GLAD pour qu'il configure OpenGL 
	gladLoadGL();

	// Je d�finis le viewport d'OpenGL dans la fen�tre
	glViewport(0, 0, 800, 800);

	// Je cr�e un objet de la class Shader et passe en param�tre
	// le chemin vers le code source du Vertex Shader et du Fragment Shader
	Shader shaderProgram("default.vert", "default.frag");

	// Je g�n�re un objet Vertex Array et le bind
	VAO VAO1;
	VAO1.Bind();

	// Je g�n�re un objet Vertex Buffer et je l'attache aux vetices
	VBO VBO1(vertices, sizeof(vertices));
	
	// Je g�n�re un objet Element Buffer et je l'attache aux indices
	EBO EBO1(indices, sizeof(indices));

	// Je link les attribus du VBO (coordonn�es et couleurs) au VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// J'unbind tout pour �viter de les modifier accidentellement
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// R�cup�rer l'ID de "l'uniforme" appel� "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// J'utilise ma classe "Texture" pour afficher une image
	Texture finn("finn.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	finn.texUnit(shaderProgram, "tex0", 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	// Boucle principale
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Pr�ciser � OpenGL quel Shader Program je veux utiliser
		shaderProgram.Activate();

		// J'attribue une valeur � l'uniforme
		// NOTE: L'attribution doit s'effectuer imp�rativement apr�s avoir activ� le Shader Program
		glUniform1f(uniID, 0.5f);

		// J'attache la texture pour qu'elle appara�sse durant le rendu
		finn.Bind();

		// J'attache le VAO pour qu'OpenGL l'utilise
		VAO1.Bind();

		// J'utilise glDrawElements au lieu de glDrawArrays, ce qui me permet de r�utiliser les sommets via les indices de l'EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		// Capture les �venements GLFW
		glfwPollEvents();
	}

	// Supprime les objets cr��s plus t�t
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	finn.Delete();
	shaderProgram.Delete();

	// Supprime la fen�tre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}