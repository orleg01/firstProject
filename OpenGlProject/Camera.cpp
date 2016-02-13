#include "Camera.h"

Camera::Camera(Shader* shader , vec3 position, vec3 up, GLfloat yaw, GLfloat pitch) : front(vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
	this->viewLoc = glGetUniformLocation(shader->program, "view");
	this->projLoc = glGetUniformLocation(shader->program, "projection");
	this->minDistanceFromObjectPossibleToSee = 0.1f;
	this->maxDistanceFromObjectPossibleToSee = 1000.0f;
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

mat4 Camera::getViewMatrix()
{
	return lookAt(this->position , this->position + this->front , this->up);
}

mat4 Camera::getPerspectiveMatrix()
{
	return perspective(this->zoom, (float)WIDTH_SCREEN / (float)HEIGHT_SCREEN, minDistanceFromObjectPossibleToSee, maxDistanceFromObjectPossibleToSee);
}

void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (direction == FORWARD) 
		this->position += this->front * velocity;
	if (direction == BACKWARD)
		this->position -= this->front * velocity;
	if (direction == LEFT)
		this->position -= this->right * velocity;
	if (direction == RIGHT)
		this->position += this->right * velocity;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;

	this->yaw   += xoffset;
	this->pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		this->zoom -= yoffset;
	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}

void Camera::setTheWorld()
{
	glUniformMatrix4fv(this->viewLoc , 1 , GL_FALSE , value_ptr(getViewMatrix()));
	glUniformMatrix4fv(this->projLoc , 1 , GL_FALSE , value_ptr(getPerspectiveMatrix()));
}

void Camera::updateCameraVectors()
{
	//Calculate the new Front vector
	vec3 front;
	front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
	front.y = sin(radians(this->pitch));
	front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));
	this->front = normalize(front);
	
	// Also need to re-calculate the Right and Up vector
	this->right = normalize(cross(this->front , this->worldUp));
	this->up    = normalize(cross(this->right , this->front));
}
