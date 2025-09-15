#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;

// Déclaration des coordonées des vertices (pyramide)
GLfloat vertices[] =
{ //	COORDONNEES		/		 COULEURS		/		UV		 /		NORMALS
	-0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,	0.0f, 0.0f,    0.0f, -1.0f,  0.0f, // Bottom side
	-0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	0.0f, 5.0f,    0.0f, -1.0f,  0.0f, // Bottom side
	 0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	5.0f, 5.0f,    0.0f, -1.0f,  0.0f, // Bottom side
	 0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,	5.0f, 0.0f,    0.0f, -1.0f,  0.0f, // Bottom side

	-0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,	0.0f, 0.0f,   -0.8f,  0.5f,  0.0f, // Left Side
	-0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	5.0f, 0.0f,   -0.8f,  0.5f,  0.0f, // Left Side
	 0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,	2.5f, 5.0f,   -0.8f,  0.5f,  0.0f, // Left Side

	-0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	5.0f, 0.0f,    0.0f,  0.5f, -0.8f, // Non-facing side
	 0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	0.0f, 0.0f,    0.0f,  0.5f, -0.8f, // Non-facing side
	 0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,	2.5f, 5.0f,    0.0f,  0.5f, -0.8f, // Non-facing side

	 0.5f,  0.0f, -0.5f,    0.83f, 0.70f, 0.44f,	0.0f, 0.0f,    0.8f,  0.5f,  0.0f, // Right side
	 0.5f,  0.0f,  0.5f,    0.83f, 0.70f, 0.44f,	5.0f, 0.0f,    0.8f,  0.5f,  0.0f, // Right side
	 0.0f,  0.8f,  0.0f,    0.92f, 0.86f, 0.76f,	2.5f, 5.0f,    0.8f,  0.5f,  0.0f, // Right side

	 0.5f,  0.0f,  0.5f,	0.83f, 0.70f, 0.44f,	5.0f, 0.0f,	   0.0f,  0.5f,  0.8f, // Facing side
 	-0.5f,  0.0f,  0.5f,	0.83f, 0.70f, 0.44f,	0.0f, 0.0f,	   0.0f,  0.5f,  0.8f, // Facing side
   	 0.0f,  0.8f,  0.0f,	0.92f, 0.86f, 0.76f,	2.5f, 5.0f,	   0.0f,  0.5f,  0.8f  // Facing side
};

// Déclaration du tableau d'indices, il permet de définir l'ordre dans lequel les sommets doivent être rendu.
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	7, 9, 8,
	10, 12, 11,
	13, 15, 14
};

// Déclaration des coordonées des vertices de ma source de lumière
GLfloat lightVertices[] =
{
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);

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
	glViewport(0, 0, width, height);

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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// J'unbind tout pour éviter de les modifier accidentellement
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Je crée un objet de la class Shader et passe en paramètre
	// le chemin vers le code source du Vertex Shader et du Fragment Shader (pour la source de lumière)
	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	// Je définis une couelur pour ma source de lumière
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Position de ma source de lumière
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// Position de ma pyramide
	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	// J'active le lightShader et j'injecte la couleur de ma source de lumière
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// J'utilise ma classe "Texture" pour afficher une image
	Texture finn("finn.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	finn.texUnit(shaderProgram, "tex0", 0);

	// J'active le "Depth Buffer"
	glEnable(GL_DEPTH_TEST);

	// Je créer l'objet Camera
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Boucle principale
	while (!glfwWindowShouldClose(window))
	{
		// Préciser la couleur du background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Vider le back buffer et assigner une nouvelle couleur (j'en profite pour clear également le depth buffer)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Je capture les inputs de la fenêtre pour intéragir avec la caméra
		camera.Inputs(window);
		// J'update et j'exporte la matrice de la caméra dans le Vertex Shader
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		
		// Préciser à OpenGL quel Shader Program je veux utiliser
		shaderProgram.Activate();

		// J'exporte la position de la caméra vers le Fragment Shader pour calculer le Specular Lightning
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		// J'exporte la camMatrix vers le Vertex Shader de la pyramide
		camera.Matrix(shaderProgram, "camMatrix");

		// J'attache la texture pour qu'elle apparaîsse durant le rendu
		finn.Bind();

		// J'attache le VAO pour qu'OpenGL l'utilise
		VAO1.Bind();

		// J'utilise glDrawElements au lieu de glDrawArrays, ce qui me permet de réutiliser les sommets via les indices de l'EBO
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		
		// J'exporte la camMatrix vers le Vertex Shader de la source de lumière
		camera.Matrix(lightShader, "camMatrix");
		
		lightVAO.Bind();

		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		// Capture les évenements GLFW
		glfwPollEvents();
	}

	// Supprime les objets créés plus tôt
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	finn.Delete();
	shaderProgram.Delete();
	
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	// Supprime la fenêtre avant de terminer le programme
	glfwDestroyWindow(window);

	// Stop GLFW avant de terminer le programme
	glfwTerminate();
	return 0;
}