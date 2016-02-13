#ifndef __OBJECT_H
#define __OBJECT_H

// GL Includes
#include <glew/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//my headers
#include "Shader.h"
#include "Texture.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct TextureStruct {
	Texture* texture;
	string type;
};

class MeshObject
{
public:
	MeshObject(vector<Vertex> vertices, vector<GLuint> indices, vector<TextureStruct> textures , Shader* shad = NULL);
	void Draw();
	void setShader(Shader* shad);
protected:
	/*Mesh Data*/
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<TextureStruct> textures;

	/* Render data */
	GLuint VAO; //vertex array object
	GLuint VBO; //vertex buffer object
	GLuint EBO; //index buffer object

	/* Shader */
	Shader* shad;

	void setUpMesh();
};

#endif