/* Header Inclusions 8*/
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

//GLM Math Header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>

using namespace std; // Standard namespace

#define WINDOW_TITLE "Modern OpenGL" // Window Title Macro

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


/*Variable declarations for shader, window size initialization, buffer and array objects*/
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture, numPoints;


/*Function prototypes*/
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);



/* Vertex Shader source code*/
const GLchar * vertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
	layout (location = 1) in vec3 color; // Color data from Vertex Attrib Pointer1

	out vec3 mobileColor; // variable to transfer color data to the fragment shader

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f);
		mobileColor = color; //references the incoming color data
	}
);


/* Fragment Shader Source Code*/
const GLchar * fragmentShaderSource = GLSL(330,
		in vec3 mobileColor; // Variable to hold incoming color data from vertex shader

		out vec4 gpuColor; //Variable to pass color data to the GPU

	void main(){

		gpuColor = vec4(mobileColor, 1.0); // Sends color data to the GPU for rendering

	}
);


/*Main Program*/
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

	// Use the Shader program
	glUseProgram(shaderProgram);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys Buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;

}

/* Resizes the window*/
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}


/* Renders graphics */
void URenderGraphics(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen



	glBindVertexArray(VAO); // Activite the Vertex Array Object before rendering and transforming them

	//Transforms the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
	model = glm::rotate(model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 35.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	// Transforms the camera
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));



	// Creates a perspective projections
	glm::mat4 projection;
	projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

	//Retrieves and passes the transform matracies to the shaper program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1 , GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	// Draw the triangles
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); // Deactivate the Vertex Array Object

	glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. Similar to GL FLUSH

}

/*Creates the Shader program*/
void UCreateShader()
{

	// Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates the Vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attaches the Vertex shader to the source code
	glCompileShader(vertexShader); // Compiles the Vertex shader

	// Fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the Fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attaches the Fragment shader to the source code
	glCompileShader(fragmentShader); // Compiles the Fragment shader

	// Shader program
	shaderProgram = glCreateProgram(); // Creates the Shader program and returns an id
	glAttachShader(shaderProgram, vertexShader); // Attach Vertex shader to the Shader program
	glAttachShader(shaderProgram, fragmentShader); // Attach Fragment shader to the Shader program
	glLinkProgram(shaderProgram); //Link Vertex and Fragment shaders to Shader program

	// Delete the Vertex and Fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


/*Creates the Buffer and Array Objects*/
void UCreateBuffers()
{

	// Position and Color data
	GLfloat vertices[] = {
				// Vertex Positions // Colors
				0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top Right Vertex 0 (red)
				0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right Vertex 1 (green)
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom Left Vertex 2 (blue)
				-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // Top Left Vertex 3 (pink)
				0.0f, 0.0f, 1.5f, 0.0f, 1.0f, 1.0f // Tip of the pyramid (teal)
				};

		// Index data to share position data
		GLuint indices[] = {
								0, 1, 3, // Triangle 1 (base1)
								1, 2, 3,  // Triangle 2 (base2)
								0, 1, 4,// Triangle 3 Side1
								0, 3, 4,// Triangle 4 Side2
								3, 2, 4,// Triangle 5 Side3
								2, 1, 4// Triangle 6 Side4
							};

		// Generate buffer ids
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Activate the Vertex Array Object before binding and setting any VBOs and Vertex Attribute Pointers.
		glBindVertexArray(VAO);

		// Activate the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copy indices to EBO

		// Activate the Elements Buffer Object / Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy indices to  EBO

		// Set attribute pointer to 0 to hold Position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0); // Enables vertexa attribute

		// Set attribute pointer 1 to hold Color data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1); // Enable vertex attribute

		glBindVertexArray(0); // Deactivates the VAO which is good practice

}
