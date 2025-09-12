#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;

// D�claration des coordon�es des vertices
GLfloat vertices[] =
{ //				COORDONNEES				/			COULEURS
	-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Coin inf�rieur droit
	 0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Coin inf�rieur gauche
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  1.0f, 0.6f,  0.32f, // Coin supp�rieur
	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Int�rieur gauche
	 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Int�rieur droit
	 0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f // Int�rieur bas
};

// D�claration du tableau d'indices, il permet de d�finir l'ordre dans lequel les sommets doivent �tre rendu.
GLuint indices[] =
{
	0, 3, 5, // Triangle inf�rieur gauche
	3, 2, 4, // Triangle inf�rieur droit
	5, 4, 1 // Triangle sup�rieur
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// J'unbind tout pour �viter de les modifier accidentellement
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// R�cup�rer l'ID de "l'uniforme" appel� "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

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

		// J'attache le VAO pour qu'OpenGL l'utilise
		VAO1.Bind();

		// J'utilise glDrawElements au lieu de glDrawArrays, ce qui me permet de r�utiliser les sommets via les indices de l'EBO
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		// Capture les �venements GLFW
		glfwPollEvents();
	}

	// Supprime les objets cr��s plus t�t
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Supprime la fen�tre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}