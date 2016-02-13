#include "Shader.h"

Shader::Shader(const GLchar * vertex,const GLchar * fragment, bool file)
{
	GLchar* vertexCode , *fragmentCode;

	if (file) {
		vertexCode = getStringFromPath(vertex);
		fragmentCode = getStringFromPath(fragment);
	}
	else 
	{
		vertexCode = strdup(vertex);
		fragmentCode = strdup(fragment);
	}

	GLuint vertexPoint, fragmentPoint;

	//compile vertex shader
	vertexPoint = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexPoint, 1, &vertexCode , NULL);
	glCompileShader(vertexPoint);

	//check if shadder compiled successfully
	if (!errorAtShader(vertexPoint , GL_COMPILE_STATUS))
		throw "vertex shader didnt compile succsessfully";

	//compile fragment shader
	fragmentPoint = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentPoint, 1, &fragmentCode, NULL);
	glCompileShader(fragmentPoint);

	//check if shadder compiled succesfully
	if (!errorAtShader(fragmentPoint , GL_COMPILE_STATUS))
		throw "fragment shader didnt compile succsessfully";

	//create program and insert the shader diffrent kind of shader into the program
	program = glCreateProgram();
	glAttachShader(program, vertexPoint);
	glAttachShader(program, fragmentPoint);
	glLinkProgram(program);

	//check if program linked successfully
	if(!errorAtShader(program , GL_LINK_STATUS))
		throw "program didnt like successfully";

	glDeleteShader(vertexPoint);
	glDeleteShader(fragmentPoint);

	delete vertexCode;
	delete fragmentCode;
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::unUse()
{
	glUseProgram(0);
}

GLchar* Shader::getStringFromPath(const GLchar * patch)
{
	// the simple variable
	std::string codeOfShader;
	std::ifstream shaderFile;

	//ensure ifstream can throw exceptions
	shaderFile.exceptions(std::ifstream::badbit);

	try 
	{
		//open file
		shaderFile.open(patch);
		std::stringstream shaderStream;

		//read file buffer contents
		shaderStream << shaderFile.rdbuf();

		//close file
		shaderFile.close();

		return strdup(shaderStream.str().c_str());

	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return nullptr;
}

bool Shader::errorAtShader(GLuint shader, GLenum mode)
{
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(shader, mode, &success);
	if (!success) 
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER\n" << infoLog << "\n" <<std::endl;
	}

	return success;
}
