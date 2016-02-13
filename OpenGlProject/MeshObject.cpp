#include "MeshObject.h"

MeshObject::MeshObject(vector<Vertex> vertices, vector<GLuint> indices, vector<TextureStruct> textures, Shader * shad)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->shad = shad;

	this->setUpMesh();
}

void MeshObject::Draw()
{
	GLuint diffuseNumber = 1;
	GLuint specularNumber = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		stringstream ss;
		string number;
		string name = this->textures[i].type;

		if (name == "texture_diffuse")
			ss << diffuseNumber++;
		else if (name == "texture_specular")
			ss << specularNumber++;
		number == ss.str();
		textures[i].texture->bind(this->shad, ("material." + name + number), i);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	Texture::unbind();
}

void MeshObject::setShader(Shader* shad)
{
	this->shad = shad;
}

void MeshObject::setUpMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
