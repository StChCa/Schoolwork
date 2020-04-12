// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Modern OPENGL"

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variable declarations for shadeer, window size buffer and array objects
GLint shaderProgram, WindowWidth = 880, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture;

// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);

// Vertex shader source code
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 color;
		layout (location = 2) in vec2 textureCoordinates;

		out vec3 mobileColor;
		out vec2 mobileTextureCoordinate;

	void main(){
		gl_Position = vec4(position.x, position.y, position.z, 1.0);
		mobileColor = color;
		mobileTextureCoordinate = vec2(textureCoordinates.x, 1.0f - textureCoordinates.y); //flips the texture
	}
);

// Fragment shader source code
const GLchar * fragmentShaderSource = GLSL(330,
		in vec3 mobileColor;
		in vec2 mobileTextureCoordinate;

		out vec4 gpuTexture;

		uniform sampler2D uTexture;

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

	glUseProgram(shaderProgram); // uses the shader program

	glClearColor(0.0f, 0.0f, -.0f, 1.0f); // Sets the bg color to black

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears screen

	glBindVertexArray(VAO); // Activate the bertex array obj

	glBindTexture(GL_TEXTURE_2D, texture); // Activate the texture

	// Draw triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};


	GLuint indices[] = {
			0,1,3, //triag 1
			1,2,3 //Triangle 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Activate the VAO before binding and settting vbos and vaps
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Activate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// set attribute pointer 1 to hold COLOR data. Note that color is not used
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // unbind the vao
}

// Generate and load the texture
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Loads texture

	cout << image;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}
