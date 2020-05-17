/*
 * ModuleOneTest.cpp
 *
 *  Created on: Mar 4, 2020
 *      Author: Stephen Cardone
 */

#include <GL/freeglut.h> // Include the freeGLUT header file
/* Implements Display Callback Handler*/
void displayGraphics()
{
/*Specify the red, green, blue, and alpha values used when the color buffers are cleared. Color is set to
Black with full Opacity.*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // Uses the color buffer and sets the background color of the window
	glFlush(); // Empties all buffers and executes all issued commands to be accepted by the rendering engine
}
/* Main function required for Immediate mode */
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Initializes the freeGLUT library
	glutCreateWindow("Stephen Cardone"); // Create a window and title (Title is my name.)
	glutInitWindowSize(640, 480); // Specifies the window's width and height
	glutInitWindowPosition(0, 0); // Specifies the position of the window's top-left corner
	glutDisplayFunc(displayGraphics); // Sets the display callback for the current window
	glutMainLoop(); // Enters the GLUT event processing loop
	return 0; // Exits main function
}
