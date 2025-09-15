#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
	public:
		// On déclare les 3 vecteurs principaux utilisé pour la position et l'orientation de la caméra
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		// Sers à empêcher le spam clique pour bouger la caméra
		bool firstClick = true;

		int width;
		int height;

		float speed = 0.04f;
		float sensitivity = 100.0f;

		// Constructeur de la caméra
		Camera(int width, int height, glm::vec3 position);

		// Update et envoi la matrice de la caméra (camMatrix) vers le Vertex Shader
		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
		// Exporte la camMatrix vers un shader
		void Matrix(Shader& shader, const char* uniform);
		// Gère les inputs de la fenêtre pour contrôler la caméra
		void Inputs(GLFWwindow* window);
};

#endif