/* Header Inclusions */
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM math header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>

using namespace std; //Standard namespace

#define WINDOW_TITLE "Modern OpenGL"

/* Shader Program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


/* Window width and height */
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600, mouseX;
GLuint VBO, VAO;

GLfloat cameraSpeed = 0.0005f; //movement speed per frame


GLfloat lastMouseX = 400, lastMouseY = 300; // Locks mouse curser at the center of screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f; // mouse offset, yaw and pitch vars
GLfloat sensitivity = 0.005f; //Used for mouse/camera rotation sensitivity
bool mouseDetected = true; // Initially true when mouse movement is detected

//Global vector declarations
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Initial camera position. Plased 5 units in Z
glm::vec3 CameraUpY = glm::vec3(0.0f, 1.0f, 0.0f); // Temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -1.0f); // Temp z unit vector
glm::vec3 front; // Temp z unit vector for mouse

/*Function prototypes*/
void UResizeWindow(int, int);
void URenderGraphics(void);
void UKeyboard(unsigned char key, int x, int y);
void UKeyReleased(unsigned char key, int x, int y);
void UMouseClick(int button, int state, int x, int y);
void UMouseMove(int x, int y);
void UMousePressedMove(int x, int y);
void UCreateShader(void);
void UCreateBuffers(void);

// fragment shader
const GLchar * fragmentShaderSource = GLSL(330,
		in vec3 mobileColor;
		out vec4 gpuColor;
	void main(){
		gpuColor = vec4(mobileColor, 1.0);
	}
);

// Vertex shader
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; //vertex data from vertex attrib pointer 0
		layout (location = 1) in vec3 color; //Color data from vertex attrib ptr 1

		out vec3 mobileColor; // var to transfer color to fragment shader

		// global vars for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); //Transform vertices to clip coordinates
		mobileColor = color; //refrences incoming color data
	}
);

/* Main Program*/
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
	glUseProgram(shaderProgram);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
	glutDisplayFunc(URenderGraphics);
	// Keyboard funcs
	glutKeyboardFunc(UKeyboard); // Detects key press
	glutKeyboardUpFunc(UKeyReleased); // Detects key release

	// Mouse functions
	glutMouseFunc(UMouseClick);
	glutPassiveMotionFunc(UMouseMove);
	glutMotionFunc(UMousePressedMove);

	glutMainLoop();

	// Destroys Buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return 0;

}

// Resizes the window
void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0,0,WindowWidth, WindowHeight);
}


// Render graphics
void URenderGraphics(void)
{

	glEnable(GL_DEPTH_TEST); //enable z-depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

	glBindVertexArray(VAO); // Activate the vertex array object before rendering and transforming

	CameraForwardZ = front; //Replaces camera forward vector with Radians normalized as unit vector

	//Transform the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Place the obj at the center of viewport
	model = glm::rotate(model, 45.0f, glm::vec3(0.0, 1.0f, 0.0f)); //Rotate the obj 45 deg on the X
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); //Increase the size by factor of 2

	//Transform the camera
	glm::mat4 view;
	view = glm::lookAt(CameraForwardZ, cameraPosition, CameraUpY);

	// Creates a perspective projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);// deactivate the vertex array obj

	glutSwapBuffers(); // Flips the back and front buffer every frame

}

// Implements the UKeyboard function
void UKeyboard(unsigned char key, GLint x, GLint y)
{
	switch(key){

	case 'w':
		cout<<"You pressed W!"<<endl;
		break;

	case 'a':
		cout<<"You pressed A!"<<endl;
		break;

	case 's':
		cout<<"You pressed S!"<<endl;
		break;

	case 'd':
		cout<<"You pressed D!"<<endl;
		break;

	default:
		cout<< "Press a key!" <<endl;
	}
}

// implements the ukeyreleased function
void UKeyReleased(unsigned char key, GLint x, GLint y)
{

	cout<<"key released!"<< endl;
}

void UMouseClick(int button, int state, int x, int y)
{
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		cout<<"Left Mouse Button Released!" << endl;
	}

	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)){
		cout<<"Left Mouse Button Released!"<<endl;
	}
}

void UCreateShader(){
	//Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attaches the Vertex shader to the source code
	glCompileShader(vertexShader); // Compiles the Vertex shader

	// Fragment Shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Creates the frag shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attaches the frag shader to the source
	glCompileShader(fragmentShader); // Compiles the frag shader

	// Shader Program
	shaderProgram = glCreateProgram(); //Creates the shader progogram and returns an id
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //Link vertex and frag shaders to program

	//Delete the vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void UMousePressedMove(int x, int y){
	cout << "Mouse pressed and moved!" << endl;
}

void UCreateBuffers()
{
	GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f
	};

	// Generate buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Activate the vertex array obj before binding and seeing any bvbos and vertex attrib pointers
	glBindVertexArray(VAO);

	//Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copy vertices to VBO)

	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enables vertex attribute

	// Set attribute pointer 1 to hold color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // Enables vertex attribute

	glBindVertexArray(0); // Deactivates the VAO which is good practice
}

void UMouseMove(int x, int y)
{
	if(mouseDetected)
	{
		lastMouseX = x;
		lastMouseY = y;
		mouseDetected = false;
	}

	// gets the direction the mouse was moved in x and y
	mouseXOffset = x - lastMouseX;
	mouseYOffset = lastMouseY - y; // Inverted Y

	// updates with new mouse coordinates
	lastMouseX = x;
	lastMouseY = y;

	// Applies sensitivity to mouse direction
	mouseXOffset *= sensitivity;
	mouseYOffset *= sensitivity;

	// Accumulates the yaw and pitch vars
	yaw += mouseXOffset;
	pitch += mouseYOffset;

	// Orbits around the center
	front.x = 10.0f * cos(yaw);
	front.y = 10.0f * sin(pitch);
	front.z = sin(yaw) * cos(pitch) * 10.0f;
}
