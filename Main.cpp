#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

using namespace std;

// Déclaration des coordonées des vertices
GLfloat vertices[] =
{ //				COORDONNEES				/			COULEURS
	-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Coin inférieur droit
	 0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f, // Coin inférieur gauche
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  1.0f, 0.6f,  0.32f, // Coin suppérieur
	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Intérieur gauche
	 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f,  0.9f, 0.45f, 0.17f, // Intérieur droit
	 0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,  0.8f, 0.3f,  0.02f // Intérieur bas
};

// Déclaration du tableau d'indices, il permet de définir l'ordre dans lequel les sommets doivent être rendu.
GLuint indices[] =
{
	0, 3, 5, // Triangle inférieur gauche
	3, 2, 4, // Triangle inférieur droit
	5, 4, 1 // Triangle supérieur
};

int main()
{
	// J'initialise la librarie GLFW
	// C'est un genre d'API pour utiliser OpenGL
	glfwInit();

	// Je précise à GLFW quelle version d'OpenGL j'utlise
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Je précise à GLFW que j'utilise le CORE profile
	// En gros c'est pour dire qu'on utilisera que les fonctions récentes
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Je crée une instance de l'objet GLFWwindow pour créer ma fenêtre
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

	// Je vérifie que la fenêtre est bien créé
	if (!window)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Je dis à OpenGL qu'il doit push le rendu sur mon objet 'window'
	glfwMakeContextCurrent(window);

	// J'initialise GLAD pour qu'il configure OpenGL 
	gladLoadGL();

	// Je définis le viewport d'OpenGL dans la fenêtre
	glViewport(0, 0, 800, 800);

	// Je crée un objet de la class Shader et passe en paramètre
	// le chemin vers le code source du Vertex Shader et du Fragment Shader
	Shader shaderProgram("default.vert", "default.frag");

	// Je génère un objet Vertex Array et le bind
	VAO VAO1;
	VAO1.Bind();

	// Je génère un objet Vertex Buffer et je l'attache aux vetices
	VBO VBO1(vertices, sizeof(vertices));
	
	// Je génère un objet Element Buffer et je l'attache aux indices
	EBO EBO1(indices, sizeof(indices));

	// Je link les attribus du VBO (coordonnées et couleurs) au VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// J'unbind tout pour éviter de les modifier accidentellement
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Récupérer l'ID de "l'uniforme" appelé "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Boucle principale
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Préciser à OpenGL quel Shader Program je veux utiliser
		shaderProgram.Activate();

		// J'attribue une valeur à l'uniforme
		// NOTE: L'attribution doit s'effectuer impérativement après avoir activé le Shader Program
		glUniform1f(uniID, 0.5f);

		// J'attache le VAO pour qu'OpenGL l'utilise
		VAO1.Bind();

		// J'utilise glDrawElements au lieu de glDrawArrays, ce qui me permet de réutiliser les sommets via les indices de l'EBO
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		// Capture les évenements GLFW
		glfwPollEvents();
	}

	// Supprime les objets créés plus tôt
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Supprime la fenêtre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}