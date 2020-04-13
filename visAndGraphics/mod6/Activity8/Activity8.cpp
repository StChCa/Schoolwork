// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

//GLM Math Header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Modern OPENGL"

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


// Variable declarations for shadeer, window size buffer and array objects
GLint cubeShaderProgram, lampShaderProgram, WindowWidth = 880, WindowHeight = 600;
GLuint VBO, CubeVAO, LightVAO, texture;

// Subject position and scale
glm::vec3 cubePosition(0.0f, -1.0f, 0.0f);
glm::vec3 cubeScale(1.2f);

// Cube light color
glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(0.7f, 1.0f, 0.7f); // slightly green for light1
glm::vec3 light2Color(1.0f, 1.0f, 1.0f); // white light from light2

// Light position and scale
glm::vec3 lightPosition(-5.0f, 5.0f, -5.0f);
glm::vec3 lightScale(1.0f); // Light 1 intensity 100%
glm::vec3 light2Position(0.0f, 0.f, 5.0f);
glm::vec3 light2Scale(0.10f); // light2 intensity 10%

// Camera position
glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);

//Camera rotation
float cameraRotation = glm::radians(65.0f);


// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UGenerateTextures(void);

// CubeVertex shader source code
const GLchar * cubeVertexShaderSource = GLSL(330,

		layout (location = 0) in vec3 position; // VAP position 0 for vertex position data
		layout (location = 1) in vec3 normal; // VAP position 1 for normals
		layout (location = 2) in vec2 textureCoordinates;

		out vec3 Normal; // For outgoing normals to gragmentshader
		out vec3 FragmentPos; // For outgoing color/pixels to fragments ahdaer
		out vec2 mobileTextureCoordniate;

		// uniform / Global variables for the transofrm matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){

		gl_Position = projection * view * model * vec4(position, 1.0f); // Transform vertices to clip coordinates

		FragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel pos in world space only

		Normal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties

		mobileTextureCoordinate = vec2(textureCoordinates.x, 1.0f - textureCoordinates.y); //flips the texture

	}
);


// Fragment shader source code
const GLchar * cubeFragmentShaderSource = GLSL(330,

		in vec3 Normal; // For incoming normals
		in vec3 FragmentPos; /// for incoming fragment posiont
		in vec2 mobileTextureCoordinate; // texture coordinates

		//out vec4 cubeColor; // for outgoing cube color to gpu
		out vec4 gpuTexture; // Variable to pass color data to GPU

		// uniform / global vars for object color, light color, light pos and camera/view pos
		uniform vec3 objectColor;
		uniform vec3 lightColor;
		uniform vec3 light2Color;
		uniform vec3 lightPos;
		uniform vec3 light2Pos;
		uniform vec3 viewPosition;
		uniform sampler2D uTexture; // Useful when working with multiple textures

	void main(){

		// Phong lighting model calculations to generate ambient, diffuse, and specular components


		/// Commented out ambient lighting because it doesn't appear to be used in the examples of activity 8
//		// Calculate ambient lighting
//		float ambientStrength = 0.1f; // Set ambient or global lighting strength
//		vec3 ambient = ambientStrength * lightColor * 1; // Generate ambient light color at 100%
//		// ambient for light 2
//		vec3 ambient2 = ambientStrength * light2Color * .1; // at 10%


		// Calculat ediffuse lighting
		vec3 norm = normalize(Normal); // Normalize vectors to 1 unit
		vec3 lightDirection = normalize(lightPos - FragmentPos); // calculate distance (light direction) between light source and fragments/pixels on
		float impact = max(dot(norm, lightDirection), 0.0); // Calcuate diffuse impact by generating dot product of normal and light
		vec3 diffuse = impact * lightColor * 1; // Generate diffuse light color at 100%

		// diffuse light2
		vec3 light2Direction = normalize(light2Pos - FragmentPos);
		float impact2 = max(dot(norm, light2Direction), 0.0f);
		vec3 diffuse2 = impact2 * light2Color * .1;// at 10%


		// Commented out for assignment as assignment does not show any specular lighting in the prompt.
		// Calculate specular lighting
//		float specularIntensity = 0.8f; // Set specular light strength
//		float highlightSize = 16.0f; // Set specular highlight size
//		vec3 viewDir = normalize(viewPosition - FragmentPos); // Calclulate view direction
//		vec3 reflectDir = reflect(-lightDirection, norm); // Calculate reflection vector
//		// Calculate specular component
//		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
//		vec3 specular = specularIntensity * specularComponent * lightColor;

		// Calculating phong result
		vec3 phong = ( diffuse + diffuse2) * gpuTexture; // to add ambient, add in ambient and ambient2 and un-comment above.

		gpuTexture = vec4(phong, 1.0f); // Send lighting result to gpu

		//gpuTexture = texture(uTexture + cubeColor, mobileTextureCoordinate); // Sends texture to the GPU

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

	UGenerateTextures();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Sets the bg color to black

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &CubeVAO);
	glDeleteVertexArrays(1, &LightVAO);
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

	GLint modelLoc, viewLoc, projLoc, objectColorLoc, lightColorLoc, lightPositionLoc, viewPositionLoc, light2ColorLoc, light2PositionLoc;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// Use the Cube shader and activate the cube vertex array obj for rendering and transformaing
	glUseProgram(cubeShaderProgram);
	glBindVertexArray(CubeVAO);

	// Transform the cube
	model = glm::translate(model, cubePosition);
	model = glm::scale(model, cubeScale);

	// Transform the camera
	view = glm::translate(view, cameraPosition);
	view = glm::rotate(view, cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));

	// Set the camera projection to perspective
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Referencec matrix uniforms from the cube shader program
	modelLoc = glGetUniformLocation(cubeShaderProgram, "model");
	viewLoc = glGetUniformLocation(cubeShaderProgram, "view");
	projLoc = glGetUniformLocation(cubeShaderProgram, "projection");

	// Pass matrix data to the cube shader program's matrix uniforms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Reference matrix uniforms forom the cube shader program for the cube color, light color, light pos and camera pos
	objectColorLoc = glGetUniformLocation(cubeShaderProgram, "objectColor");
	lightColorLoc = glGetUniformLocation(cubeShaderProgram, "lightColor");
	light2ColorLoc = glGetUniformLocation(cubeShaderProgram, "light2Color");
	lightPositionLoc = glGetUniformLocation(cubeShaderProgram, "lightPos");
	light2PositionLoc = glGetUniformLocation(cubeShaderProgram, "light2Pos");
	viewPositionLoc = glGetUniformLocation(cubeShaderProgram, "viewPosition");

	// Pass color, light, and camera data to the cube shader program's corresponding uniforms
	glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(light2ColorLoc, light2Color.r, light2Color.g, light2Color.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(light2PositionLoc, light2Position.x, light2Position.y, light2Position.z);
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the primitives /cube

	glBindVertexArray(0); // Deactivate the Cube Vertex Array object

	glutPostRedisplay();

	glutSwapBuffers();


}

// Creates the shader program
void UCreateShader()
{

	// Cube Vertex shader
	GLint cubeVertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the vertex shader
	glShaderSource(cubeVertexShader, 1, &cubeVertexShaderSource, NULL); // attaches vert shader to source
	glCompileShader(cubeVertexShader); // Compiles the vertex shader

	// Cube Fragment shader
	GLint cubeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create frag shader
	glShaderSource(cubeFragmentShader, 1, &cubeFragmentShaderSource, NULL); //Attaches the frag shader to source
	glCompileShader(cubeFragmentShader); //compiles frag shader

	// Cube Shader program
	cubeShaderProgram = glCreateProgram(); //Creates the shader program returns an id
	glAttachShader(cubeShaderProgram, cubeVertexShader); //Attach vertex shader to shader prog
	glAttachShader(cubeShaderProgram, cubeFragmentShader);
	glLinkProgram(cubeShaderProgram);

	// Delete the vertex and fragment shaders once linked
	glDeleteShader(cubeVertexShader);
	glDeleteShader(cubeFragmentShader);
}


// Creates the buffer and array objs
void UCreateBuffers()
{

	GLfloat vertices[] = {


			//pos x, y, z// Neg z normal // texture points
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Back
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			 0.0f,  0.75f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.75f,

			 // Pos z normal
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Front
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
			 0.0f,  0.75f,  0.0f,  0.0f, 0.0f,  1.0f, 0.5f, 0.75f,

			 // Neg X normal
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Left
			-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f,  0.75f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f, 0.75f,

			// Pos x normal
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Right
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.75f,  0.0f,  1.0f, 0.0f,  0.0f, 0.5f, 0.75f,

			// - y notmal
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom of pyramid
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	};


	// Generate buffer ids for cube
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &VBO);

	// Activate the VAO before binding and settting vbos and vaps
	glBindVertexArray(CubeVAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enables vertex attribute

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Deactivate VAO

	// generate buffer id's for lamp
	glGenVertexArrays(1, &LightVAO);

	//activate the vao before binding
	glBindVertexArray(LightVAO);

	// Reference the same vbo for verticies
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set attribute pointer to 0 to hold pos data of lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

// Generate and load the texture
void UGenerateTextures(){

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;

	unsigned char* image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGB); // Loads texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
}
