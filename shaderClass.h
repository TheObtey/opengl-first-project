#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		// L'ID de r�f�rence du Shader Program
		GLuint ID;
		// Constructeur qui assemble le Shader Program � partir de deux shaders (Vertex et Fragment shader)
		Shader(const char* vertexFile, const char* fragmentFile);

		// Active le Shader Program
		void Activate();
		// Supprime le Shader Program
		void Delete();
	private:
		// V�rifie que les shaders ont compil� correctement
		void compileErrors(unsigned int shader, const char* type);
};

#endif