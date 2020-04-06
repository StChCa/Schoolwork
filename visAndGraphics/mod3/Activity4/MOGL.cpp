// Include header files and i/o stream
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <cmath>

using namespace std; // Use the standar namespace

#define WINDOW_TILE "Modern OpenGL" // Macro for window title

// Vertex and Fragment Shader Source Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variables for window width and height
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture, numPoints;

/* User-defined Function prototypes to:
 * initialize the program, st the window size,
 * redraw graphics on the window when resized,
 * and render graphics on thes creen
 */
void UInitialize(int, char*[]);
void UInitWindow(int, char*[]);
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateVBO(void);
void UCreateShaders(void);
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateBuffers(void);
void test(vector<GLfloat>&, vector<GLuint>&);
void createCircle(int, int, GLfloat, GLfloat, GLfloat, vector<GLfloat>&, vector<GLuint>&);

// Vertex Shader Program Source Code
const GLchar * VertexShader = GLSL(440,
	in layout(location=0) vec4 vertex_Position;
	// Get the vertex colors from the vertex buffer object
	in layout(location=1) vec4 colorFromVBO; // for attribute 1 expect vec(4) floats passed into the vertex shader
	out vec4 colorFromVShader; // declare a vec 4 variable that will reference the vertex colors passed into the vertex shader from the buffer
		void main(){
			gl_Position = vertex_Position; // Sends vertex positions to gl_position vec4
			colorFromVShader = colorFromVBO; // references vertex colors sent from the buffer
		}
	);

// Fragment Shader Program Code
const GLchar * FragmentShader = GLSL(440,
	in vec4 colorFromVShader; // Vertex colors from the vertex shader
	out vec4 vertex_Color; // Vec 4 variable that will reference the vertex colors passed into the fragment shader from the vertex shader
		void main(){
//	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Sets the pixels / fragments of the triangle to green
		vertex_Color = colorFromVShader; // Send the vertex  colors to the GPU
	}
);

// main function. Entry point to the OpenGL program
int main(int argc, char* argv[]){
		UInitialize(argc, argv); // Initialize the OpenGL program
		glutMainLoop(); // Starts the OpenGL loop in the background
		exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Implements the UInitialize function
void UInitialize(int argc, char* argv[]){
	// glew status variable
	GLenum GlewInitResult;

	UInitWindow(argc,argv);

	// Checks glew status
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult){
		fprintf(stderr,"ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	// Displays GPU OpenGL version
	fprintf(stdout,"INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

	UCreateVBO(); // Calls the function to create the Vertex Buffer Object

	UCreateShaders(); // Calls the function to create the Shader Program

	UCreateBuffers();

	//Sets the background color of the window to black. Optional
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}

// Implements the UInitWindow function
void UInitWindow(int argc, char* argv[]){
	// Initalizes freeglut
	glutInit(&argc, argv);

	// Sets the window size
	glutInitWindowSize(WindowWidth, WindowHeight);

	// Memory buffer setup for display
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Creates a window with the macro placeholder title
	glutCreateWindow(WINDOW_TILE);


	glutReshapeFunc(UResizeWindow); // Called when the window is resized
	glutDisplayFunc(URenderGraphics); // Renders graphics on the screen
}

void UResizeWindow(int Width, int Height){
	glViewport(0,0,Width,Height);
}

// Implements the URenderGraphics function
void URenderGraphics(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

	// Crates teh triangle
	GLuint totalVerticies = 6; // Specifies the number of verticies for the triangle i.e 3
	cout << totalVerticies;
	//glDrawArrays(GL_TRIANGLES, 0, totalVerticies); // Draws the Triangle

	// Draw the triangles using the indicies
	glDrawElements(GL_TRIANGLES, totalVerticies, GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. Similar to GL Flush
}

// Implements the Create VBO function
void UCreateVBO(void){
	// Specifies coordinates for triangle verticies on X and Y
	GLfloat verts[] = {
			// Index 1 will be shared by both triangles.

			// Index 0
			-1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f, // RED

			// Index 1 (Used by both triangles)
			-0.5f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f, // GREEN

			// Index 2
			-1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f, // BLUE

			// Index 3
			0.0, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, // RED

			// Index 4
			0.0f, -1.0f,
			0.0f, 1.0f, 0.0f, 1.0f, // GREEN

//			0.0f, -1.0f,
//			0.0f, 0.0f, 1.0f, 1.0f
	};

	// Stores the size of the verts array / number of coordinates needed for the triangle i.e 6
	float numVerticies = sizeof(verts);

	GLuint myBufferID; // Variable for vertex buffer object id
	glGenBuffers(1, &myBufferID); // Creates 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID); // Activiates the buffer
	glBufferData(GL_ARRAY_BUFFER, numVerticies, verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Creates the Vertex Attribute Pointer
	GLuint floatsPerVertex = 2; // Number of coordinates per vertex
	glEnableVertexAttribArray(0); // Specifies the initial position of the coordinates in the buffer
	/* Instructs the GPU on how to handle the vertex buffer object data.
	 * Parameters: attribPointerPosition | coordinates per vertex | data type | deactivate normalization | 0 strides | o offset
	 */
	GLint vertexStride = sizeof(float) * 6; // The number of floats before each vertex position is 6 i.e. xy rgba
	/* Instructs the GPU on how to handle the vertex buffer object data.
	 * Parameters: attribPointerPosition | coordinates per vertex is 2 i.e. x andy | data type | deactivate normalization | 0 strides | offset
	 */
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, vertexStride, 0); // change the stride from 0 to 6

	// Sets an attribute pointer position for the vertex colors i.e. Attribute 1 for rgba floats. Attribute 0 was for position x,y
	glEnableVertexAttribArray(1); // Specifies position 1 for the color values in the buffer

	GLint colorStride = sizeof(float) * 6; // The number of floats before each color is 6 i.e. rgba xy

	/* Parameters : attribPointerPosition 1 | floats per color is 4 i.e. rgba | data type | deactivate normalizatoin |
	 * 6 strides until the next color i.e. rgbaxy | 2 floats until the beginning of each color
	 */
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE, colorStride, (char*)(sizeof(float) * 2));

	/* Creates a buffer object for the indicies */
	GLushort indicies[] = {0,1,2,3,4,1}; // Using index 1 twice
	float numIndicies = sizeof(indicies);
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies, indicies, GL_STATIC_DRAW);
}

// Implements the UCreateShaders function
void UCreateShaders(void){
	// Create a shader Program object.
	GLuint ProgramId = glCreateProgram();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); //Create a vertex shader object
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object

	glShaderSource(vertexShaderId, 1, &VertexShader, NULL); // retrieves the vertex shader source code
	glShaderSource(fragmentShaderId, 1, &FragmentShader, NULL); // retrieves the fragment shader source code

	glCompileShader(vertexShaderId); // Compile the vertex shader
	glCompileShader(fragmentShaderId); // Compile the fragment shader

	// Attaches the vertex and fragment shaders to the shader program
	glAttachShader(ProgramId, vertexShaderId);
	glAttachShader(ProgramId, fragmentShaderId);

	glLinkProgram(ProgramId); // Links the Shader program
	glUseProgram(ProgramId); // Uses the shader program
}

/*Creates the Buffer and Array Objects*/
void UCreateBuffers()
{

	// Position and Color data
	vector<GLfloat> verticesVec;
	vector<GLuint> polygonsVec;

//	verticesVec.reserve(30);
//	polygonsVec.reserve(18);

	//test(verticesVec, polygonsVec);

	createCircle(1, 4, 0.0f, 0.0f, 0.0f, verticesVec, polygonsVec);

	// Print polys for testing
	for (unsigned int i = 0; i < verticesVec.size(); i++){
		cout << verticesVec[i];
		cout << "  ";
	}
	cout << endl;
	for (unsigned int i = 0; i < polygonsVec.size(); i++){
		cout << polygonsVec[i];
		cout << "  ";
	}

		// Convert vectors into arrays for drawing
		GLfloat vertices[verticesVec.size()];
		for (unsigned int i = 0; i < verticesVec.size(); i++){
			vertices[i] = verticesVec[i];
		}

		GLuint indices[polygonsVec.size()];
		for (unsigned int i = 0; i < polygonsVec.size(); i++){
			indices[i] = polygonsVec[i];
		}

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

void createCircle(int radius, int numPoints, GLfloat centX, GLfloat centY, GLfloat centZ, vector<GLfloat>& verts, vector<GLuint>& polys){
	GLuint prevSize = verts.size() / 6;
	float theta = 360.0f / (float)numPoints;

	// Push a point for the center point @ prevSize + 0
	verts.push_back(centX);
	verts.push_back(centY);
	verts.push_back(centZ);
	verts.push_back(1.0f);
	verts.push_back(0.0f);
	verts.push_back(0.0f);

	// Generate points on the circle using basic trig
	for (int i = 0; i < numPoints; i++){
		// get degrees (theta) for current point
		float deg = theta * i;

		verts.push_back(cos(deg) * radius);
		verts.push_back(sin(deg) * radius);
		verts.push_back(centZ);
		verts.push_back(1.0f);
		verts.push_back(0.0f);
		verts.push_back(0.0f);

	}

	// Create a triangle between center, x and x-1 points on the circle (start with pt 2)
	for (int i = 2; i < numPoints + 2; i++){

		// push centerOfCircle point
		polys.push_back(prevSize + 0);
		polys.push_back(prevSize + i);
		polys.push_back(prevSize + i - 1);
		//break;
	}

}


















