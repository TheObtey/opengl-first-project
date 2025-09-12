#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

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

	// Définis une couleur pour le background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Je vide le back buffer et lui assigne la nouvelle couleur
	glClear(GL_COLOR_BUFFER_BIT);

	// J'intervertis le back buffer avec le front buffer
	glfwSwapBuffers(window);

	// Boucle principale
	while (!glfwWindowShouldClose(window))
	{
		// Capture les évenements GLFW
		glfwPollEvents();
	}

	// Supprime la fenêtre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}