// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <ctime>
//GLM Math Header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>
// Soil
#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Cardone Final"

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

/***********************************************************************************
 * Variable Declarations
 ***********************************************************************************/
GLint shaderProgram, lampShaderProgram;
GLint WindowWidth = 880, WindowHeight = 600; // window sizes
GLuint VBO, lightVBO, VAO, lightVAO, texture; // vertex objects and texture
GLfloat degrees = glm::radians(-45.0f); // Converts float to degrees
GLfloat lastMouseX = 400, lastMouseY = 300; // Locks mouse curser at the center of screen
GLfloat mouseXOffset, mouseYOffset, yaw = 0.0f, pitch = 0.0f; // mouse offset, yaw and pitch vars
GLfloat sensitivity = 0.01f; //Used for mouse/camera rotation sensitivity
bool mouseDetected = true, rightClicked = false, leftClicked = false, autoRotate = true, orthoPerspective = false; // Initially true when mouse movement is detected
// Vector declarations
glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(1.0f, 0.8f, 0.8f);
glm::vec3 lightPosition(5.0f, 2.0f, 5.0f);
glm::vec3 lightScale(0.3f);
// camera declarations
glm::vec3 cameraPosition(0.0f, 0.0f, -0.0f);
glm::vec3 CameraUpY = glm::vec3(0.0f, 1.0f, 0.0f); // Temporary y unit vector
glm::vec3 CameraForwardZ = glm::vec3(0.0f, 0.0f, -10.0f); // Temp z unit vector
glm::vec3 front; // Temp z unit vector for mouse
float cameraRotation = glm::radians(-25.0f);

/***********************************************************************************
 * FUNCTION PROTOTYPES
 ***********************************************************************************/
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);

// Mouse and camera movement prototypes
void orbit(int x, int y);
void UMouseMove(int x, int y);
void UMouseEntryFunc(int);
void UKeyboard(unsigned char key, int x, int y);
void UKeyReleased(unsigned char key, int x, int y);
void toggleOrtho();

// Drawing and shaping prototypes
void drawCircle(float, int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, vector<GLfloat>&);
void drawConnectedCircles(vector<GLfloat>, vector<GLfloat>, GLfloat, GLfloat, GLfloat, vector<GLfloat>&);
void createEqualTriangle(float, float, float, float, float, vector<GLfloat>&);
void createEdgesOfTriangle(vector<GLfloat>&);
void createRectangleZ(float, float, float, float, float, float, vector<GLfloat>&);
void createRectangleY(float, float, float, float, float, float, vector<GLfloat>&);
void createRectangleX(float, float, float, float, float, float, vector<GLfloat>&);
vector<GLfloat> generateCircleVerts(float, int, GLfloat, GLfloat, GLfloat);
vector<GLfloat> generateOffsetVec(vector<GLfloat>);
void createHandle(vector<GLfloat>&);

/***********************************************************************************
 * Shader source
 ***********************************************************************************/
// Vertex shader source code
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; // Vertex data from vertex attrib pointer 0
		layout (location = 1) in vec3 normal; // Normal data for lighting
		layout (location = 2) in vec2 textureCoordinates; // texture locations

		out vec3 FragmentPos;
		out vec3 Normal;
		out vec2 mobileTextureCoordinate;

		// Global variables for the transform matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transform vertices to clip coordinates
		FragmentPos = vec3(model * vec4(position, 1.0f));
		Normal = mat3(transpose(inverse(model))) * normal;
		mobileTextureCoordinate = vec2(textureCoordinates.x, 1 - textureCoordinates.y); //flips the texture
	}
);


// Fragment shader source code
const GLchar * fragmentShaderSource = GLSL(330,

		in vec3 FragmentPos;
		in vec3 Normal;
		in vec2 mobileTextureCoordinate;

		out vec4 pyramidColor;
		//out vec4 gpuTexture; // Variable to pass color data to GPU

		uniform vec3 lightColor;
		uniform vec3 lightPos;
		uniform vec3 viewPosition;

		uniform sampler2D uTexture; // Useful when working with multiple textures

	void main(){

		//gpuTexture = texture(uTexture, mobileTextureCoordinate); // Sends texture to the GPU

		float ambientStrength = 0.4f; // value to set strength of ambient
		vec3 ambient = ambientStrength * lightColor; // Apply the strength to the color

		vec3 norm = normalize(Normal); // Normaliz to 1 unit
		vec3 lightDirection = normalize(lightPos - FragmentPos); // Calculate distance beween light source and fragments/pixels
		float impact = max(dot(norm, lightDirection), 0.0); // Calculate diffuse impact by generating dot product of normal light
		vec3 diffuse = impact * lightColor; // Generate diffuse light color

		float specularIntensity = 0.9f; // Set specular light strength
		float highlightSize = 500.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - FragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(lightDirection, norm); // calculate reflection vector

		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize); //Specular component
		vec3 specular = specularIntensity * specularComponent * lightColor;

		// Calculate the phong result
		vec3 objectColor = texture(uTexture, mobileTextureCoordinate).xyz;
		vec3 phong = (ambient + diffuse) * objectColor + specular;
		pyramidColor = vec4(phong, 1.0f); // Send lighting result to GPU


	}
);

// Lamp shader source
const GLchar * lampVertexShaderSource = GLSL(330,

		layout (location = 0) in vec3 position;

		// Uniform /global variables for the transorm matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices into clip coordintes

	}
);

// Fragment shader source
const GLchar * lampFragmentShaderSource = GLSL(330,

		out vec4 color; //For outgoing lamp color (light source) tothe GPU

	void main(){
		color = vec4(1.0f, 0.8f, 0.8f, 1.0f); // Set lightColor
	}
);

/***********************************************************************************
 * Main Program
 ***********************************************************************************/
int main(int argc, char* argv[])
{
	// Seed random number to allow for variability in some textures
	srand ( time(NULL) );

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

	UCreateShader(); // Call our shader creation function

	UCreateBuffers(); // Call our buffer creation function

	UGenerateTexture(); // Call texture generator function

	// Use the Shader program
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Sets the bg color to black

	glutDisplayFunc(URenderGraphics); // set our graphics display function

	// Keyboard / mouse functions
	glutPassiveMotionFunc(UMouseMove); // Passive mouse movement callback
	glutEntryFunc(UMouseEntryFunc); // Mouse enter/exit callback
	glutKeyboardFunc(UKeyboard); // Detects key press
	glutKeyboardUpFunc(UKeyReleased); // Detects key release

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &lightVBO);

	return 0;
}

/***********************************************************************************
 * Graphics functions
 ***********************************************************************************/
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

	CameraForwardZ = front; // Replaces camera forward vecor twith radians normalized as unit vector
	// initialize vars
	GLint modelLoc, viewLoc, projLoc, lightColorLoc, lightPositionLoc, viewPositionLoc, uTextureLoc;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glUseProgram(shaderProgram); // Set shader program to be used
	glBindVertexArray(VAO); // Activate the bertex array obj

	// Transofms  the object
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); // Place the object in center of biewport
	if (autoRotate){
		model = glm::rotate(model, glutGet(GLUT_ELAPSED_TIME) * .0005f, glm::vec3(1.0, 1.0f, 0.0f)); //roatate y -45 degrees
	}
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the size by scale 2

	// Transforms the camera
	//view = glm::translate(view, glm::vec3(0.0f, -2.0f, -10.0f)); // Moves the workd .5 units on x and -5 units in z

	// Transform view based on mouse movement
	view = glm::lookAt(CameraForwardZ, cameraPosition, CameraUpY);

	// Creates a perspective projection
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to theshader program
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Lighting uniform info to apply to pan
	lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	uTextureLoc = glGetUniformLocation( shaderProgram, "uTexture");
	glUniform1i(uTextureLoc, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 10000);

	glBindVertexArray(0);

	glm::mat4 model1(1.0f);
	glUseProgram(lampShaderProgram);
	glBindVertexArray(lightVAO);

	// transform light as needed.
	model1 = glm::translate(model1, lightPosition);
	model1 = glm::scale(model1, lightScale);

	// Uniforms for light source
	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 10000);

	glBindVertexArray(0);

	glutPostRedisplay();
	glutSwapBuffers();

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


	// Same process for lamp
	// Vertex shader
	GLint lampVertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the vertex shader
	glShaderSource(lampVertexShader, 1, &lampVertexShaderSource, NULL); // attaches vert shader to source
	glCompileShader(lampVertexShader); // Compiles the vertex shader

	// Fragment shader
	GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create frag shader
	glShaderSource(lampFragmentShader, 1, &lampFragmentShaderSource, NULL); //Attaches the frag shader to source
	glCompileShader(lampFragmentShader); //compiles frag shader

	// Shader program
	lampShaderProgram = glCreateProgram(); //Creates the shader program returns an id
	glAttachShader(lampShaderProgram, lampVertexShader); //Attach vertex shader to shader prog
	glAttachShader(lampShaderProgram, lampFragmentShader);
	glLinkProgram(lampShaderProgram);

	// Delete the vertex and fragment shaders once linked
	glDeleteShader(lampVertexShader);
	glDeleteShader(lampFragmentShader);

}

// Creates the buffer and array objs
void UCreateBuffers()
{
	// Store light cube vertexes
	GLfloat lightVerts[] ={
			0.0f, 0.0f, 0.0f, // front origin
			1.0f, 1.0f, 0.0f, // front Up right
			1.0f, 0.0f, 0.0f, // front lower right

			0.0f, 0.0f, 0.0f, // front origin
			1.0f, 1.0f, 0.0f, // front Up right
			0.0f, 1.0f, 0.0f, // front up left

			1.0f, 1.0f, 0.0f, // front Up right
			0.0f, 1.0f, 0.0f, // front up left
			0.0f, 1.0f, 1.0f, // back top left

			1.0f, 1.0f, 0.0f, // front Up right
			0.0f, 1.0f, 1.0f, // back top left
			1.0f, 1.0f, 1.0f, // back top right

			0.0f, 1.0f, 1.0f, // back top left
			1.0f, 1.0f, 1.0f, // back top right
			1.0f, 0.0f, 1.0f, // back bottom right

			0.0f, 1.0f, 1.0f, // back top left
			0.0f, 0.0f, 1.0f, // back origin
			1.0f, 0.0f, 1.0f, // back bottom right

			0.0f, 0.0f, 0.0f, // front origin
			0.0f, 0.0f, 1.0f, // back origin
			1.0f, 0.0f, 1.0f, // back bottom right

			0.0f, 0.0f, 0.0f, // front origin
			1.0f, 0.0f, 1.0f, // back bottom right
			1.0f, 0.0f, 0.0f, // front lower right

			0.0f, 0.0f, 0.0f, // front origin
			0.0f, 0.0f, 1.0f, // back origin
			0.0f, 1.0f, 0.0f, // front up left

			0.0f, 0.0f, 1.0f, // back origin
			0.0f, 1.0f, 0.0f, // front up left
			0.0f, 1.0f, 1.0f, // back top left

			1.0f, 0.0f, 0.0f, // front lower right
			1.0f, 0.0f, 1.0f, // back bottom right
			1.0f, 1.0f, 1.0f, // back top right

			1.0f, 0.0f, 0.0f, // front lower right
			1.0f, 1.0f, 0.0f, // front Up right
			1.0f, 1.0f, 1.0f, // back top right
	};

	/**********************************
	 * Find pan vertexes
	 *********************************/
	// Position and Color data
	vector<GLfloat> verticesVec;

	vector<GLfloat> lowerPanCircle = generateCircleVerts(.85, 30, 0.0f, 0.0f, 0.0f);
	vector<GLfloat> upperPanCircle = generateCircleVerts(1.1, 30, 0.0f, 0.0f, 0.3f);
	vector<GLfloat> bottomLowerPanCircle = generateCircleVerts(.85, 30, 0.0f, 0.0f, -0.1f);

	drawConnectedCircles(lowerPanCircle, upperPanCircle, 0.0f, 0.0f, 1.0f, verticesVec);
	drawConnectedCircles(bottomLowerPanCircle, upperPanCircle, 0.0f, 0.0f, -1.0f, verticesVec);

	//	// Base of pan
	drawCircle(.85, 30, 0.0f, 0.0f, -0.1f, 0.0f, 0.0f, -1.0f, verticesVec);
	drawCircle(.85, 30, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, verticesVec);

	// end of handle
	createHandle(verticesVec);

	// base of handle
	createEqualTriangle(0.6f, 0.0f, 1.3f, 0.3f, 1.0f, verticesVec);
	createEqualTriangle(0.6f, 0.0f, 1.3f, 0.21f, -1.0f, verticesVec);
	createEdgesOfTriangle(verticesVec);

	// Convert vectors into arrays for drawing
	GLfloat vertices[verticesVec.size()];
	for (unsigned int i = 0; i < verticesVec.size(); i++){
		vertices[i] = verticesVec[i];
	}

	/*******************
	 *  DRAW LIGHT
	 ******************/
	// Generate buffer ids for lamp
	glGenVertexArrays(2, &lightVAO);
	glGenBuffers(2, &lightVBO);

	//Activate the lamp vao
	glBindVertexArray(lightVAO);

	// referenceing the same vbo for its verticies
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVerts), lightVerts, GL_STATIC_DRAW);

	// set the attrib pointer to 0 to hold position data used for lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(1);

	/*******************
	 *  DRAW PAN
	 ******************/
	// Generate buffer ids
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Activate the VAO before binding and settting vbos and vaps
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // unbind the vao

}

/***********************************************************************************
 * Drawing and shape creation functions
 ***********************************************************************************/
void createHandle(vector<GLfloat>& verts){
	//				L1,     l2,    x,    y,     z,    normal, verts
	createRectangleZ(0.25f, 0.75f, 0.0f, 1.75f, 0.3f, 1.0f, verts);
	createRectangleZ(0.25f, 0.75f, 0.0f, 1.75f, 0.2f, -1.0f, verts);
	createRectangleX(0.75f, 0.10f, 0.125f, 1.75f, 0.25f, 1.0f, verts);
	createRectangleX(0.75f, 0.10f, -0.125f, 1.75f, 0.25f, -1.0f, verts);
	createRectangleY(0.25f, 0.10f, 0.0f, 2.125f, .25f, 1.0f, verts);
}

// Generate and load the texture
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;
	// use the gold7.jpg from roject root.
	unsigned char* image = SOIL_load_image("gold7.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Loads texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}

// Rectangle for each orientation
void createRectangleZ(float xSide, float ySide, float x, float y, float z, float norm, vector<GLfloat>& verts){
	// It will take 2 triangles to draw this rectangle

	// Draw triagnle 1
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back(z);
	// Normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Draw triagnle 2
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

}

void createRectangleX(float ySide, float zSide, float x, float y, float z, float norm, vector<GLfloat>& verts){
	// It will take 2 triangles to draw this rectangle

	// Draw triagnle 1
	verts.push_back(x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back( (zSide/2.0f) + z);
	// Normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back(x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back( (-zSide/2.0f) + z);
	// normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back(x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back( (-zSide/2.0f) +z);
	// normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Draw triagnle 2
	verts.push_back(x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back( (-zSide/2.0f) + z);
	// normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back(x);
	verts.push_back( (-ySide/2.0f) + y);
	verts.push_back( (zSide/2.0f) + z);
	// normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back(x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back( (zSide/2.0f) + z);
	// normals
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

}

void createRectangleY(float xSide, float zSide, float x, float y, float z, float norm, vector<GLfloat>& verts){
	// It will take 2 triangles to draw this rectangle

	// Draw triagnle 1
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (zSide/2.0f + z));
	// Normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (zSide/2.0f + z));
	// normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (-zSide/2.0f + z));
	// normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Draw triagnle 2
	verts.push_back( (-xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (-zSide/2.0f + z));
	// normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (-zSide/2.0f + z));
	// normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back(y);
	verts.push_back( (zSide/2.0f + z));
	// normals
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

}

// Draw an equalateral triangle in a given position
void createEqualTriangle(float size, float x, float y, float z, float norm, vector<GLfloat>& verts){

	// Point 1 of triangle
	verts.push_back(0.0f + x);
	verts.push_back( (size/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Point2 of triangle
	verts.push_back( (size/2.0f) + x);
	verts.push_back( (-size/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	// Point 3 of triangle
	verts.push_back( (-size/2.0f) + x);
	verts.push_back( (-size/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f * norm);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);
}

// draw the edges of the pan pommel area (sides of triangle)
void createEdgesOfTriangle(vector<GLfloat>& verts){

	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.3f);
	//normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	//T2,
	verts.push_back(0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.3f);
	verts.push_back(0.98f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Side 2
	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.3f);
	//normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(-0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	//T2,
	verts.push_back(-0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(-0.3f);
	verts.push_back(0.98f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.0f);
	verts.push_back(1.3f + 0.3f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(-0.5f);
	verts.push_back(0.5f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);


	// Create side 3
	verts.push_back(-0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(-0.3f);
	verts.push_back(0.98f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.3f);
	verts.push_back(0.98f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(0.3f);
	verts.push_back(0.98f);
	verts.push_back(0.21f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

	verts.push_back(-0.3f);
	verts.push_back(1.0f);
	verts.push_back(0.3f);
	// normals
	verts.push_back(0.0f);
	verts.push_back(-1.0f);
	verts.push_back(0.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

}

// Draw a circle of a given radius, point density, position, normal
void drawCircle(float radius, int numPoints, GLfloat centX, GLfloat centY, GLfloat centZ, GLfloat norm1, GLfloat norm2, GLfloat norm3, vector<GLfloat>& verts){

	float theta = 360.0f / (float)numPoints;

	// for each triangle we will draw (num points = num triangles
	for (int i = 1; i < numPoints+1; i++){

		float pt1Deg = theta * (i-1);
		float pt2Deg = theta * i;

		float pt1Rad = pt1Deg * (3.141592653589793238463/180.0);
		float pt2Rad = pt2Deg * (3.141592653589793238463/180.0);

		cout << i << " pt1Rad " << pt1Rad<<endl;
		cout << i << " pt2Rad " << pt2Rad << endl << endl;

		// Add center point to this triangle
		verts.push_back(centX);
		verts.push_back(centY);
		verts.push_back(centZ);
		// normals
		verts.push_back(norm1);
		verts.push_back(norm2);
		verts.push_back(norm3);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// Add a point for the i-1 point
		verts.push_back(cos(pt1Rad) * radius);
		verts.push_back(sin(pt1Rad) * radius);
		verts.push_back(centZ);
		// normals
		verts.push_back(norm1);
		verts.push_back(norm2);
		verts.push_back(norm3);
		// texture
		verts.push_back(cos(pt1Rad) * 1.0f);
		verts.push_back(sin(pt1Rad) * 1.0f);

		// Add a point for this current point
		verts.push_back(cos(pt2Rad) * radius);
		verts.push_back(sin(pt2Rad) * radius);
		verts.push_back(centZ);
		// normals
		verts.push_back(norm1);
		verts.push_back(norm2);
		verts.push_back(norm3);
		// texture
		verts.push_back(cos(pt2Rad) * 1.0f);
		verts.push_back(sin(pt2Rad) * 1.0f);
	}
}

// Generate a circle and return the vector of points on the circle
vector<GLfloat> generateCircleVerts(float radius, int numPoints, GLfloat centX, GLfloat centY, GLfloat centZ){

	// vector data to return
	vector<GLfloat> circle;

	float theta = 360.0f / (float)numPoints;

	for (int i = 1; i < numPoints+1; i++){

		float pt1Deg = theta * (i-1);

		float pt1Rad = pt1Deg * (3.141592653589793238463/180.0);

		// Add a point for the i-1 point
		circle.push_back(cos(pt1Rad) * radius);
		circle.push_back(sin(pt1Rad) * radius);
		circle.push_back(centZ);
	}

	return circle;
}

// Connect the exterior points of 2 circles
void drawConnectedCircles(vector<GLfloat> c1, vector<GLfloat> c2, GLfloat normX, GLfloat normY, GLfloat normZ, vector<GLfloat>& verts){

	// we need to alternate drawing points on c1 and c2 like this
	// draw triangle of points c1[0], c1[1], c2[0]
	// then c2[0], c2[1], c1[1]
	// we will use modulous to make sure we don't go over max number of verts
	float theta = 360.0f / (float)c1.size();

	for ( unsigned int i = 0; i < c1.size(); i+=3 ){
		//Triangle 1
		// pt1 (c1[i])
		int current = i;
		int next = (i+3)%c1.size();
		verts.push_back(c1[current]); //x
		verts.push_back(c1[current+1]); //y
		verts.push_back(c1[current+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// pt2 (c1[i+1]
		verts.push_back(c1[next]); //x
		verts.push_back(c1[next+1]); //y
		verts.push_back(c1[next+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// pt3 (c2[i])
		verts.push_back(c2[current]); //x
		verts.push_back(c2[current+1]); //y
		verts.push_back(c2[current+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);


		// Triangle 2
		//c2[i]
		verts.push_back(c2[current]); //x
		verts.push_back(c2[current+1]); //y
		verts.push_back(c2[current+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// c2[i+1]
		verts.push_back(c2[next]); //x
		verts.push_back(c2[next+1]); //y
		verts.push_back(c2[next+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// pt3 (c1[i+1])
		verts.push_back(c1[next]); //x
		verts.push_back(c1[next+1]); //y
		verts.push_back(c1[next+2]); //z
		// normals
		verts.push_back(cos(theta * (i/3) * normX));
		verts.push_back(sin(theta * (i/3) * normY));
		verts.push_back(normZ);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);
	}
}

/***********************************************************************************
 * Keyboard/Mouse functions
 ***********************************************************************************/
void UMouseEntryFunc(int state){
	if( state == GLUT_LEFT) {
		// Start auto rotate
		autoRotate = true;
		std::cout << "LEFT" << endl;
	} else if( state == GLUT_ENTERED){
		// Stop auto rotate.
		autoRotate = false;
		std::cout << "Entered" << endl;
	}
}

void UMouseMove(int x, int y){
	orbit(x, y);
}

void orbit(int x, int y){
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
	front.z = sin(yaw) * cos(pitch) * 10.0f;

	// clamp pitch to prevent straing camera movement
	if( pitch < -1 || pitch > 1 ) {
		return;
	}
	front.y = 10.0f * sin(pitch);
}

void UKeyboard(unsigned char key, int x, int y){

	switch(key){
		case 'p':
			toggleOrtho();
			cout<<"P pressed, toggle ortho perspective" << endl;
			break;
		default:
			cout<<"No key";
	}

}
void UKeyReleased(unsigned char key, int x, int y){
	// Not used
}

void toggleOrtho(){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(!orthoPerspective){
		std::cout << "set to ortho" << endl;
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1, 10);
		orthoPerspective = true;
	}else {
		glFrustum(-1.0, 2.0, -1.0, 1.0, 5, 100);
		orthoPerspective = false;
		std::cout << "set to frustum" << endl;
	}

	glMatrixMode(GL_MODELVIEW);

}
