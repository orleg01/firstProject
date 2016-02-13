#ifndef __SHADER_H
#define __SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew\GL\glew.h>

class Shader
{
public:
	// final shader object id
	GLuint program;
	// constructor for shader in file
	Shader(const GLchar* vertex,const GLchar* fragment , bool file = true);
	// deconstructore
	~Shader();
	// use the shader
	void use();
	// stop Using the shader
	static void unUse();
protected:
	GLchar* getStringFromPath(const GLchar* patch);
	bool errorAtShader(GLuint shader, GLenum mode);
};


#endif