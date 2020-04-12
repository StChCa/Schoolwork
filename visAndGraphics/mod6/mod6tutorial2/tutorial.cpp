// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// math
//GLM Math Header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>

// Soil
#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Modern OPENGL"

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variable declarations for shadeer, window size buffer and array objects
GLint shaderProgram, WindowWidth = 880, WindowHeight = 600;
GLuint VBO, VAO, texture;
GLfloat degrees = glm::radians(-45.0f); // Converts float to degrees

// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);


// Vertex shader source code
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; // Vertex data from vertex attrib pointer 0
		layout (location = 2) in vec2 textureCoordinates;

		out vec2 mobileTextureCoordinate;


		// Global variables for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transform vertices to clip coordinates
		mobileTextureCoordinate = vec2(textureCoordinates.x, 1.0f - textureCoordinates.y); //flips the texture
	}
);


// Fragment shader source code
const GLchar * fragmentShaderSource = GLSL(330,

		in vec2 mobileTextureCoordinate;

		out vec4 gpuTexture; // Variable to pass color data to GPU

		uniform sampler2D uTexture; // Useful when working with multiple textures

	void main(){

		gpuTexture = texture(uTexture, mobileTextureCoordinate); // Sends texture to the GPU

	}
);


// main program
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);


	glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
				return -1;
			}

	UCreateShader();

	UCreateBuffers();

	UGenerateTexture();

	// Use the Shader program
	glUseProgram(shaderProgram); // uses the shader program

	glClearColor(0.0f, 0.0f, -.0f, 1.0f); // Sets the bg color to black

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;
}

// Resizes the window
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0,0, WindowWidth, WindowHeight);
}


// Renders Graphics
void URenderGraphics(void)
{

	glEnable(GL_DEPTH_TEST); // Enable z-depth

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears screen

	glBindVertexArray(VAO); // Activate the bertex array obj

	// Transofms  the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); // Place the object in center of biewport
	model = glm::rotate(model, degrees, glm::vec3(0.0, 1.0f, 0.0f)); //roatate y -45 degrees
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the size by scale 2

	// Transforms the camera
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)); // Moves the workd .5 units on x and -5 units in z

	// Creates a perspective projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to theshader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	glutPostRedisplay();

	glBindTexture(GL_TEXTURE_2D, texture); // Activate the texture

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0); // Deactivate the VAO

	glutSwapBuffers(); // Flips the back buffer to the front every frame

}

// Creates the shader program
void UCreateShader()
{

	// Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attaches vert shader to source
	glCompileShader(vertexShader); // Compiles the vertex shader

	// Fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create frag shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //Attaches the frag shader to source
	glCompileShader(fragmentShader); //compiles frag shader

	// Shader program
	shaderProgram = glCreateProgram(); //Creates the shader program returns an id
	glAttachShader(shaderProgram, vertexShader); //Attach vertex shader to shader prog
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete the vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}



// Creates the buffer and array objs
void UCreateBuffers()
{

	GLfloat vertices[] = {
			//pos // color //texture coordinates
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};






































	// Generate buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Activate the VAO before binding and settting vbos and vaps
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // unbind the vao

}

// Generate and load the texture
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Loads texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}
