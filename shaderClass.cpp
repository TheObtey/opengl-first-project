#include"shaderClass.h"

// Fonction pour r�cup�rer le contenu d'un fichier
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
	// R�cup�rer le code source depuis les fichiers afin de constuire le Shader Program
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

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
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Je compile le code source du vertexShader en code machine
	glCompileShader(vertexShader);

	// V�rifier si il y a eu des probl�mes de compilation
	compileErrors(vertexShader, "VERTEX");

	// M�me �tapes pour le Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	compileErrors(fragmentShader, "FRAGMENT");

	/*
		Un shader program c�est un programme qu'on envoie
		� la carte graphique pour lui dire exactement comment traiter tes donn�es.
		Il est compos� du Vertex Shader et du Fragment Shader.
	*/

	// Je cr�e un objet r�f�rence pour le Shader Program
	ID = glCreateProgram();

	// J'attache le Vertex et Fragment Shader � mon Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// J'attache/Link tout les shaders enssemble � l'int�rieur de mon Shader Program
	glLinkProgram(ID);

	compileErrors(ID, "PROGRAM");

	// Maintenant que j'ai un Shader Program, je supprime les deux shaders qui n'ont plus d'utilit�
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