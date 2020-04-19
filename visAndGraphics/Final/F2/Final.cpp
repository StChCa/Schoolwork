// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <ctime>

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
GLint shaderProgram, lampShaderProgram, WindowWidth = 880, WindowHeight = 600;
GLuint VBO, VAO, lightVAO, texture;
GLfloat degrees = glm::radians(-45.0f); // Converts float to degrees

glm::vec3 pyramidPosition(0.0f, 0.0f, 0.0f);
glm::vec3 pyramidScale(1.0f);

glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 lightPosition(5.0f, 2.0f, 5.0f);
glm::vec3 lightScale(0.3f);

// camera position
glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);

float cameraRotation = glm::radians(-25.0f);

// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTexture(void);
void drawCircle(float, int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, vector<GLfloat>&);
vector<GLfloat> generateCircleVerts(float, int, GLfloat, GLfloat, GLfloat);
void drawConnectedCircles(vector<GLfloat>, vector<GLfloat>, GLfloat, GLfloat, GLfloat, vector<GLfloat>&);
void createEqualTriangle(float, float, float, float, vector<GLfloat>&);
void createRectangle(float, float, float, float, float, vector<GLfloat>&);
vector<GLfloat> generateOffsetVec(vector<GLfloat>);
void createHandle(vector<GLfloat>&);


// Vertex shader source code
const GLchar * vertexShaderSource = GLSL(330,
		layout (location = 0) in vec3 position; // Vertex data from vertex attrib pointer 0
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 textureCoordinates;

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

		float specularIntensity = 0.3f; // Set specular light strength
		float highlightSize = 128.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - FragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-lightDirection, norm); // calculate reflection vector

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

		out vec4 color; //For outgoing lamp color (smaller pyramid) tothe GPU

	void main(){
		color = vec4(1.0f); // Set color to white with alpha 1.0
	}
);

// main program
int main(int argc, char* argv[])
{
	// Seed random number to allow for variability in textures
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

	UCreateShader();

	UCreateBuffers();

	UGenerateTexture();

	// Use the Shader program
	//glUseProgram(shaderProgram); // uses the shader program

	glClearColor(0.0f, 0.0f, -.0f, 1.0f); // Sets the bg color to black

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
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



	GLint modelLoc, viewLoc, projLoc, lightColorLoc, lightPositionLoc, viewPositionLoc, uTextureLoc;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // Activate the bertex array obj

	// Transofms  the object
	model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); // Place the object in center of biewport
	model = glm::rotate(model, glutGet(GLUT_ELAPSED_TIME) * .0005f, glm::vec3(0.0, 1.0f, 0.0f)); //roatate y -45 degrees
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the size by scale 2

	// Transforms the camera
	view = glm::translate(view, glm::vec3(0.0f, -2.0f, -10.0f)); // Moves the workd .5 units on x and -5 units in z

	// Creates a perspective projection
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to theshader program
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPos");
	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	uTextureLoc = glGetUniformLocation( shaderProgram, "uTexture");
	glUniform1i(uTextureLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, 10000);

	glBindVertexArray(0);

	glm::mat4 model1(1.0f);
	glUseProgram(lampShaderProgram);
	glBindVertexArray(lightVAO);


	model1 = glm::translate(model1, lightPosition);
	model1 = glm::scale(model1, lightScale);

	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 10000);

	glBindVertexArray(0);

	glUseProgram(lampShaderProgram);

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
	glShaderSource(lampVertexShader, 1, &vertexShaderSource, NULL); // attaches vert shader to source
	glCompileShader(lampVertexShader); // Compiles the vertex shader

	// Fragment shader
	GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create frag shader
	glShaderSource(lampFragmentShader, 1, &fragmentShaderSource, NULL); //Attaches the frag shader to source
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
//	// Top of pan (opening)
//	drawCircle(1.1, 30, 0.0f, 0.0f, 0.3f, verticesVec);
	// base of handle
	createEqualTriangle(0.6f, 0.0f, 1.3f, 0.3f, verticesVec);
	createEqualTriangle(0.6f, 0.0f, 1.3f, 0.25f, verticesVec);
	// end of handle
	createHandle(verticesVec);

	// Create a second copy of verticesVec. Offset it in the Z by a few px and draw all the connecting poinst
	//vector<GLfloat> offsetVec = generateOffsetVec(verticesVec);

	// drwa the connection triangles from original to offset shape.
//	drawConnectedCircles(offsetVec, verticesVec, verticesVec);

	// append offsetVec to verticesVec
//	for (int i = 0; i < offsetVec.size(); i++){
//		verticesVec.push_back(offsetVec[i]);
//	}

		// Convert vectors into arrays for drawing
		GLfloat vertices[verticesVec.size()];
		for (unsigned int i = 0; i < verticesVec.size(); i++){
			vertices[i] = verticesVec[i];
		}

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

	// Generate buffer ids for lamp
	glGenVertexArrays(1, &lightVAO);

	//Activate the lamp vao
	glBindVertexArray(lightVAO);

	// referenceing the same vbo for its verticies
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// set the attrib pointer to 0 to hold position data used for lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);

}

void createHandle(vector<GLfloat>& verts){
	createRectangle(0.25f, 0.75f, 0.0f, 1.75f, 0.3f, verts);
	createRectangle(0.25f, 0.75f, 0.0f, 1.75f, 0.25f, verts);
	createRectangle(0.25f, 0.75f, 0.0f, 1.75f, 0.3f, verts);
	createRectangle(0.25f, 0.75f, 0.0f, 1.75f, 0.3f, verts);
	createRectangle(0.25f, 0.75f, 0.0f, 1.75f, 0.3f, verts);
}

vector<GLfloat> generateOffsetVec(vector<GLfloat> inVec){
	vector<GLfloat> offsetVec;
	std::cout << inVec.size();

	for (unsigned int i = 0; i < inVec.size(); i++){
		std::cout << i << " val : " << inVec[i];
		if ( i % 6 == 5){
			offsetVec.push_back( 0.0f );
		} else{
			offsetVec.push_back(inVec[i]);
		}
	}
	return offsetVec;
}

// Generate and load the texture
void UGenerateTexture(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("gold7.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Loads texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}

void createRectangle(float xSide, float ySide, float x, float y, float z, vector<GLfloat>& verts){
	// It will take 2 triangles to draw this rectangle

	// Draw triagnle 1
	verts.push_back( (xSide/2.0f) + x);
	verts.push_back( (ySide/2.0f) + y);
	verts.push_back(z);
	// Normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);

}

void createEqualTriangle(float size, float x, float y, float z, vector<GLfloat>& verts){

	// Point 1 of triangle
	verts.push_back(0.0f + x);
	verts.push_back( (size/2.0f) + y);
	verts.push_back(z);
	// normals
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
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
	verts.push_back(1.0f);
	// texture
	verts.push_back((float)(rand() % 100) / 100.0f);
	verts.push_back((float)(rand() % 100) / 100.0f);
}

void drawCircle(float radius, int numPoints, GLfloat centX, GLfloat centY, GLfloat centZ, GLfloat norm1, GLfloat norm2, GLfloat norm3, vector<GLfloat>& verts){

	float theta = 360.0f / (float)numPoints;

//	// Push a point for the center point @ prevSize + 0
//	verts.push_back(centX);
//	verts.push_back(centY);
//	verts.push_back(centZ);
//	// Color info
//	verts.push_back(1.0f);
//	verts.push_back(0.0f);
//	verts.push_back(0.0f);
//
//	// Generate points on the circle using basic trig
//	for (int i = 0; i < numPoints; i++){
//		// get degrees (theta) for current point
//		float deg = theta * i;
//
//		verts.push_back(cos(deg) * radius);
//		verts.push_back(sin(deg) * radius);
//		verts.push_back(centZ);
//		verts.push_back(1.0f);
//		verts.push_back(0.0f);
//		verts.push_back(0.0f);
//
//	}

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
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);

		// Add a point for this current point
		verts.push_back(cos(pt2Rad) * radius);
		verts.push_back(sin(pt2Rad) * radius);
		verts.push_back(centZ);
		// normals
		verts.push_back(norm1);
		verts.push_back(norm2);
		verts.push_back(norm3);
		// texture
		verts.push_back((float)(rand() % 100) / 100.0f);
		verts.push_back((float)(rand() % 100) / 100.0f);
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
