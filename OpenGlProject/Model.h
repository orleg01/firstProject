#ifndef __MODEL_H
#define __MODEL_H


#include <assimps\assimp/Importer.hpp>
#include <assimps\assimp/scene.h>
#include <assimps\assimp/postprocess.h>
#include <glew\GL\glew.h>
#include <glfw\GLFW\glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// my Headers
#include "MeshObject.h"
#include "Shader.h"
#include "Transform.h"

using namespace std;

class Model
{
public:
	Model(GLchar* path , Shader* shad);
	void Draw();
private:
	/* Model Data */
	Transform transf;
	vector<MeshObject> meshes;
	string directory;
	

	void loadModel(string path);
	void setShader(Shader* shad);
	void setShaderAtSpecificMesh(Shader* shad, GLuint index);
	void processNode(aiNode* node, const aiScene* scene);
	MeshObject processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,string typeName);

};

#endif