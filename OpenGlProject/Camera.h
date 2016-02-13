#ifndef __CAMERA_H
#define __CAMERA_H

// Std. Includes
#include <vector>

// GL Includes
#include <glew/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// data stucture
#include "UsableData.h"
#include "Shader.h"


using namespace glm;

enum Camera_Movement {
	FORWARD ,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0F;

class Camera
{
public:
	//Constructor with vectors
	Camera(Shader* shader , vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	//Constructor with scalar values
	Camera(Shader* shader, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :Camera( shader , vec3(posX, posY, posZ), vec3(upX, upY, upZ), yaw, pitch) {}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, GLfloat deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(GLfloat yoffset);

	// Set theRelevantMatrixToSader
	void setTheWorld();

private:
	//Camera Attributes
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	//Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	//Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	//Camera Distance
	GLfloat minDistanceFromObjectPossibleToSee;
	GLfloat maxDistanceFromObjectPossibleToSee;

	//ids 
	GLuint viewLoc;
	GLuint projLoc;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	mat4 getViewMatrix();

	// Return the perspective matrix
	mat4 getPerspectiveMatrix();
};

#endif