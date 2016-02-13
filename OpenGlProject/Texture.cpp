#include "Texture.h"

Texture::Texture(const char * path)
{
	// create the texture
	glGenTextures(1, &textureId);
	//declare it as 2d
	glBindTexture(GL_TEXTURE_2D, textureId);

	//set texture to repeat itself 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set the texture filter(pixel filter to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//import the texture
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// set the texture as opengl object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//create as much mipmap as it need
	glGenerateMipmap(GL_TEXTURE_2D);
	//free image from soil
	SOIL_free_image_data(image);

	//to ensure my self the texture will not be demage
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

void Texture::bind(Shader* shad, std::string nameOfTexture , int numberOfTexture)
{
	glActiveTexture(GL_TEXTURE0 + numberOfTexture);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(glGetUniformLocation(shad->program , nameOfTexture.str().c_str() ), numberOfTexture);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
