#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Code source du Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Code source du Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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

	// Déclaration des coordonées des vertices
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Coin inférieur gauche
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Coin inférieur droit
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Coin suppérieur
	};

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

	/*
		Le vertex shader s’occupe des sommets des objets.
		Chaque point qui compose un modèle passe dedans, et lui va calculer
		sa position finale (par exemple appliquer les matrices de transformation, projection, etc.).
		Il peut aussi préparer des infos comme la couleur ou
		les coordonnées de texture pour les passer plus loin.

		Le fragment shader  travaille après.
		Chaque pixel candidat à l’écran (aussi appelé “fragment”) passe dedans,
		et c’est lui qui décide de la couleur finale affichée à cet endroit.
		C’est avec lui qu'on gère la lumière, les textures, les ombres, etc.
	*/

	// Je crée un objet référence pour le Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// J'attache le code source du Vertex Shader à mon objet vertexShader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Je compile le code source du vertexShader en code machine
	glCompileShader(vertexShader);

	// Même étapes pour le Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/*
		Un shader program c’est un programme qu'on envoie
		à la carte graphique pour lui dire exactement comment traiter tes données.
		Il est composé du Vertex Shader et du Fragment Shader. 
	*/

	// Je crée un objet référence pour le Shader Program
	GLuint shaderProgram = glCreateProgram();

	// J'attache le Vertex et Fragment Shader à mon Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// J'attache/Link tout les shaders enssemble à l'intérieur de mon Shader Program
	glLinkProgram(shaderProgram);

	// Maintenant que j'ai un Shader Program, je supprime les deux shaders qui n'ont plus d'utilité
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		Le VBO stocke les données brutes des sommets (positions, couleurs, UV, etc).
		Le VAO décrit comment lire ces données et les relier aux attributs du shader.
	*/

	// Je crée une référence à des conteneurs pour le Vertex Array Object (VAO) et le Vertex Buffer Object (VBO)
	GLuint VAO, VBO;

	// Génère le VAO et VBO avec 1 objet chacun
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Je dit à OpenGL que le VAO qu'il doit utilisé c'est l'object VAO que j'ai créé
	glBindVertexArray(VAO);

	// J'attache le VBO en précisant que c'est un GL_ARRAY_BUFFER
	// En gros, je dis à OpenGL de considéré mon conteneur VBO comme un tableau de vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copie de mon VBO de la mémoire CPU vers la mémoire GPU
	/*
		GL_ARRAY_BUFFER: Dit à OpenGL que le buffer qu'on remplit, c'est un VBO de type données de sommets.
		sizeof(vertices): La taille totale des données à envoyé (en octets).
		vertices: L'adresse du tableau en RAM qui contient les sommets.
		GL_STATIC_DRAW: C'est un hint pour OpenGL, pour lui dire que les données vont peu changer (optimisation).
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// J'configure le Vertex Attribute afin qu'OpenGL sache comment lire les données du VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// J'active le Vertex Attribue en plaçant un 0 dans le pipeline, ainsi OpenGL comprend qu'il faut l'utiliser
	glEnableVertexAttribArray(0);

	// Attacher le VAO et le VBO à 0 pour éviter de modifier accidentellement le VAO ou le VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Définis une couleur pour le background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Je vide le back buffer et lui assigne la nouvelle couleur
	glClear(GL_COLOR_BUFFER_BIT);

	// J'intervertis le back buffer avec le front buffer
	glfwSwapBuffers(window);

	// Boucle principale
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Préciser à OpenGL quel Shader Program utiliser
		glUseProgram(shaderProgram);

		// J'attache le VAO pour qu'OpenGL l'utilise
		glBindVertexArray(VAO);

		// je dessine le triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		// Capture les évenements GLFW
		glfwPollEvents();
	}

	// Supprime les objets créés plus tôt
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Supprime la fenêtre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}