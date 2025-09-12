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

	// Je pr�cise � GLFW quelle version d'OpenGL j'utlise
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Je pr�cise � GLFW que j'utilise le CORE profile
	// En gros c'est pour dire qu'on utilisera que les fonctions r�centes
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// D�claration des coordon�es des vertices
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Coin inf�rieur gauche
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Coin inf�rieur droit
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Coin supp�rieur
	};

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

	/*
		Le vertex shader s�occupe des sommets des objets.
		Chaque point qui compose un mod�le passe dedans, et lui va calculer
		sa position finale (par exemple appliquer les matrices de transformation, projection, etc.).
		Il peut aussi pr�parer des infos comme la couleur ou
		les coordonn�es de texture pour les passer plus loin.

		Le fragment shader  travaille apr�s.
		Chaque pixel candidat � l��cran (aussi appel� �fragment�) passe dedans,
		et c�est lui qui d�cide de la couleur finale affich�e � cet endroit.
		C�est avec lui qu'on g�re la lumi�re, les textures, les ombres, etc.
	*/

	// Je cr�e un objet r�f�rence pour le Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// J'attache le code source du Vertex Shader � mon objet vertexShader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Je compile le code source du vertexShader en code machine
	glCompileShader(vertexShader);

	// M�me �tapes pour le Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/*
		Un shader program c�est un programme qu'on envoie
		� la carte graphique pour lui dire exactement comment traiter tes donn�es.
		Il est compos� du Vertex Shader et du Fragment Shader. 
	*/

	// Je cr�e un objet r�f�rence pour le Shader Program
	GLuint shaderProgram = glCreateProgram();

	// J'attache le Vertex et Fragment Shader � mon Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// J'attache/Link tout les shaders enssemble � l'int�rieur de mon Shader Program
	glLinkProgram(shaderProgram);

	// Maintenant que j'ai un Shader Program, je supprime les deux shaders qui n'ont plus d'utilit�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		Le VBO stocke les donn�es brutes des sommets (positions, couleurs, UV, etc).
		Le VAO d�crit comment lire ces donn�es et les relier aux attributs du shader.
	*/

	// Je cr�e une r�f�rence � des conteneurs pour le Vertex Array Object (VAO) et le Vertex Buffer Object (VBO)
	GLuint VAO, VBO;

	// G�n�re le VAO et VBO avec 1 objet chacun
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Je dit � OpenGL que le VAO qu'il doit utilis� c'est l'object VAO que j'ai cr��
	glBindVertexArray(VAO);

	// J'attache le VBO en pr�cisant que c'est un GL_ARRAY_BUFFER
	// En gros, je dis � OpenGL de consid�r� mon conteneur VBO comme un tableau de vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copie de mon VBO de la m�moire CPU vers la m�moire GPU
	/*
		GL_ARRAY_BUFFER: Dit � OpenGL que le buffer qu'on remplit, c'est un VBO de type donn�es de sommets.
		sizeof(vertices): La taille totale des donn�es � envoy� (en octets).
		vertices: L'adresse du tableau en RAM qui contient les sommets.
		GL_STATIC_DRAW: C'est un hint pour OpenGL, pour lui dire que les donn�es vont peu changer (optimisation).
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// J'configure le Vertex Attribute afin qu'OpenGL sache comment lire les donn�es du VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// J'active le Vertex Attribue en pla�ant un 0 dans le pipeline, ainsi OpenGL comprend qu'il faut l'utiliser
	glEnableVertexAttribArray(0);

	// Attacher le VAO et le VBO � 0 pour �viter de modifier accidentellement le VAO ou le VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// D�finis une couleur pour le background
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

		// Pr�ciser � OpenGL quel Shader Program utiliser
		glUseProgram(shaderProgram);

		// J'attache le VAO pour qu'OpenGL l'utilise
		glBindVertexArray(VAO);

		// je dessine le triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		// Capture les �venements GLFW
		glfwPollEvents();
	}

	// Supprime les objets cr��s plus t�t
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Supprime la fen�tre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}