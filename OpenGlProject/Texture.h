#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <soil\SOIL.h>
#include <glew\GL\glew.h>
#include <string>
#include <iostream>
#include "Shader.h"

class Texture 
{
public:
	//the id of the texture
	GLuint textureId;

	Texture(const char* path);

	~Texture();
	//bind the texture to use
	void bind(Shader* shad , std::string nameOfTexture , int numberOfTexture = 0);
	static void unbind();
};

#endif