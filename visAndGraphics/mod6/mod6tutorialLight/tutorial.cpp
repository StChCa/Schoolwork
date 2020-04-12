// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

//GLM Math Header inclusions
#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>

using namespace std;

#define WINDOW_TITLE "Modern OPENGL"

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


// Variable declarations for shadeer, window size buffer and array objects
GLint cubeShaderProgram, lampShaderProgram, WindowWidth = 880, WindowHeight = 600;
GLuint VBO, CubeVAO, LightVAO;

// Subject position and scale
glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);
glm::vec3 cubeScale(2.0f);

// Cube light color
glm::vec3 objectColor(0.6f, 0.5f, 0.75f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// Light position and scale
glm::vec3 lightPosition(0.5f, 0.5f, -3.0f);
glm::vec3 lightScale(0.3f);

// Camera position
glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);

//Camera rotation
float cameraRotation = glm::radians(-25.0f);


// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);

// CubeVertex shader source code
const GLchar * cubeVertexShaderSource = GLSL(330,

		layout (location = 0) in vec3 position; // VAP position 0 for vertex position data
		layout (location = 1) in vec3 normal; // VAP position 1 for normals

		out vec3 Normal; // For outgoing normals to gragmentshader
		out vec3 FragmentPos; // For outgoing color/pixels to fragments ahdaer

		// uniform / Global variables for the transofrm matrices
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

	void main(){

		gl_Position = projection * view * model * vec4(position, 1.0f); // Transform vertices to clip coordinates

		FragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel pos in world space only

		Normal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties

	}
);


// Fragment shader source code
const GLchar * cubeFragmentShaderSource = GLSL(330,

		in vec3 Normal; // For incoming normals
		in vec3 FragmentPos; /// for incoming fragment posiont

		out vec4 cubeColor; // for outgoing cube color to gpu

		// uniform / global vars for object color, light color, light pos and camera/view pos
		uniform vec3 objectColor;
		uniform vec3 lightColor;
		uniform vec3 lightPos;
		uniform vec3 viewPosition;

	void main(){

		// Phong lighting model calculations to generate ambient, diffuse, and specular components

		// Calculate ambient lighting
		float ambientStrength = 0.1f; // Set ambient or global lighting strength
		vec3 ambient = ambientStrength * lightColor; // Generate ambient light color


		// Calculat ediffuse lighting
		vec3 norm = normalize(Normal); // Normalize vectors to 1 unit
		vec3 lightDirection = normalize(lightPos - FragmentPos); // calculate distance (light direction) between light source and fragments/pixels on
		float impact = max(dot(norm, lightDirection), 0.0); // Calcuate diffuse impact by generating dot product of normal and light
		vec3 diffuse = impact * lightColor; // Generate diffuse light color


		// Calculate specular lighting
		float specularIntensity = 0.8f; // Set specular light strength
		float highlightSize = 16.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - FragmentPos); // Calclulate view direction
		vec3 reflectDir = reflect(-lightDirection, norm); // Calculate reflection vector
		// Calculate specular component
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		vec3 specular = specularIntensity * specularComponent * lightColor;

		// Calculating phong result
		vec3 phong = (ambient + diffuse + specular) * objectColor;

		cubeColor = vec4(phong, 1.0f); // Send lighting result to gpu
	}
);


// Lamp shader source code
const GLchar * lampVertexShaderSource = GLSL(330,

	layout (location = 0 ) in vec3 position; //VAP position 0 for vertex position data

	// Uniform/global vars for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // transform vertices into clip coordintaes
	}
);


// Fragment shader source code
const GLchar * lampFragmentShaderSource = GLSL(330,

	out vec4 color; // For outgoing lamp color (smaller cupe) to the GPU

	void main()
	{
		color = vec4(1.0f); // Set color to white (1.0f, 1.0f, 1.0f, with alpha 1.0)

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

	GLint modelLoc, viewLoc, projLoc, objectColorLoc, lightColorLoc, lightPositionLoc, viewPositionLoc;

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
	lightPositionLoc = glGetUniformLocation(cubeShaderProgram, "lightPos");
	viewPositionLoc = glGetUniformLocation(cubeShaderProgram, "viewPosition");

	// Pass color, light, and camera data to the cube shader program's corresponding uniforms
	glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
	glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(viewPositionLoc, cameraPosition.z, cameraPosition.y, cameraPosition.z);

	glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the primitives /cube

	glBindVertexArray(0); // Deactivate the Cube Vertex Array object


	// **** USE the lamp shader and activate the lamp vertex array obj for rendering
	glUseProgram(lampShaderProgram);
	glBindVertexArray(LightVAO);

	// Transform the saller cube used as a visual que for the light source
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, lightScale);

	// Reference matrix uniforms from the Lamp Shader program
	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");

	// Pass matrix data to the lamp shader program's matrix unifroms
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the primitives /small cube(lamp)

	glBindVertexArray(0); // Cleamnup

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


	// lamp Vertex shader
	GLint lampVertexShader = glCreateShader(GL_VERTEX_SHADER); //Creates the vertex shader
	glShaderSource(lampVertexShader, 1, &lampVertexShaderSource, NULL); // attaches vert shader to source
	glCompileShader(lampVertexShader); // Compiles the vertex shader

	// lamp Fragment shader
	GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create frag shader
	glShaderSource(lampFragmentShader, 1, &lampFragmentShaderSource, NULL); //Attaches the frag shader to source
	glCompileShader(lampFragmentShader); //compiles frag shader

	// lamp Shader program
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

	GLfloat vertices[] = {


			//pos // color //texture coordinates
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,


			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,


			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,


			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,


			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,


			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		 	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enables vertex attribute

	// set attribute pointer 2 to hold texture coordinate data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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
