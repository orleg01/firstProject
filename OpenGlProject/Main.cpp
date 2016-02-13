#include <stdlib.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew\GL\glew.h>
#include <glfw\GLFW\glfw3.h>
#include <assimps\assimp\Importer.hpp>
#include <soil\SOIL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define VER_SHADER(a) #a
// Other includes
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "UsableData.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_Movement();
void setMouseInFirstFrame(GLFWwindow* window);

//Camera 
Camera* camera;

static bool yes = false;

//Time andler
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH_SCREEN, HEIGHT_SCREEN, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//make the mouse cursor disapear but still function  
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH_SCREEN, HEIGHT_SCREEN);
	glEnable(GL_DEPTH_TEST); // make depth test

	//set cersure of the mouse in the middle of the screen
	setMouseInFirstFrame(window);

	// Build and compile our shader program
	Shader ourShader("./shaders/shader.vert", "./shaders/shader.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
		0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // Top Right
		0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f  // Top Left 

	};



	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3,  // Second Triangle
		0, 1, 5,
		0, 4, 5,
		1, 2, 5,
		2, 5, 6,
		0, 3, 4,
		3, 4, 7,
		2, 3, 6,
		3, 6, 7,
		4, 5, 6,
		4, 6, 7
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	Texture texture("./Image/wall.jpg");
	Texture texture2("./Image/awesomeface.png");
	
	Transform transform(&ourShader);

	camera = &Camera(&ourShader ,vec3(0.0f, 0.0f, 3.0f));
	lastFrame = glfwGetTime();
									 // Game loop
	while (!glfwWindowShouldClose(window))
	{
		//create delta-Time options and variables
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_Movement();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Activate shader
		ourShader.use();
		transform.rotate(vec3(1, 1, 0), 0.01f);
		camera->setTheWorld();
		transform.setModel();

		// Bind Texture
		texture.bind(&ourShader);
		texture2.bind(&ourShader , 1);


		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//unbind The texture
		Texture::unbind();

		//unuse shader
		Shader::unUse();

		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Moves/alters the camera positions based on user input
void do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera->processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->processKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLfloat xoffset = xpos - MOUSE_LAST_POS_X;
	GLfloat yoffset = MOUSE_LAST_POS_Y - ypos;  // Reversed since y-coordinates go from bottom to left

	MOUSE_LAST_POS_X = xpos;
	MOUSE_LAST_POS_Y = ypos;

	camera->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->processMouseScroll(yoffset);
}

void setMouseInFirstFrame(GLFWwindow* window)
{
	glfwSetCursorPos(window, MOUSE_LAST_POS_X, MOUSE_LAST_POS_Y);
}
