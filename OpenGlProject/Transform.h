#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include <glew\GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace glm;

class Transform
{
public:
	//id of transform
	GLuint transformId;

	//default constractor
	Transform(Shader* shad);
	//normal constructor
	Transform(vec3 position, vec3 scale, Shader* shad , quat rotation = quat(0, 0, 0, 1));

	~Transform();

	//move in the world
	void translate(vec3 howMuchToMove);

	//size the object length
	void changeSize(vec3 multiplyByScales);

	//rotate the object
	void rotate(vec3 Axis , float dgree);

	//modle matrix
	void setModel();

private:
	vec3 position;
	vec3 scale;
	quat rotation;

	//setters
	void inline setPos(vec3 pos);
	void inline setScale(vec3 scale);
	void inline setRotation(quat rotation);
};


#endif