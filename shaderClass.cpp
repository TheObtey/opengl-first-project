#include"shaderClass.h"

// Fonction pour récupérer le contenu d'un fichier
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Récupérer le code source depuis les fichiers afin de constuire le Shader Program
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

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
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Je compile le code source du vertexShader en code machine
	glCompileShader(vertexShader);

	// Vérifier si il y a eu des problèmes de compilation
	compileErrors(vertexShader, "VERTEX");

	// Même étapes pour le Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	compileErrors(fragmentShader, "FRAGMENT");

	/*
		Un shader program c’est un programme qu'on envoie
		à la carte graphique pour lui dire exactement comment traiter tes données.
		Il est composé du Vertex Shader et du Fragment Shader.
	*/

	// Je crée un objet référence pour le Shader Program
	ID = glCreateProgram();

	// J'attache le Vertex et Fragment Shader à mon Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// J'attache/Link tout les shaders enssemble à l'intérieur de mon Shader Program
	glLinkProgram(ID);

	compileErrors(ID, "PROGRAM");

	// Maintenant que j'ai un Shader Program, je supprime les deux shaders qui n'ont plus d'utilité
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1014, NULL, infoLog);
			std::cout << "SHADER_COMPILATOIN_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1014, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}