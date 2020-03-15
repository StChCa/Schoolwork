#include <GL/freeglut.h>
#include <iostream>

void drawPentagon(){

	// Draw a pentagon on the unit plane. Scale it down and move it to one side
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Move it .5 units to the left
	glTranslatef(-0.5f, 0.0f, 0.0f);
	// Scale it down to 30%
	glScalef(0.3f,0.3f,0.3f);
	// Draw the actual pentagon.
	glBegin(GL_POLYGON);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 1.0f); // top
		glVertex2f(1.0f, 0.3f); // Right
		glVertex2f(0.6f, -1.f); // Bottom right
		glVertex2f(-0.6f, -1.0f); // Bottom Left
		glVertex2f(-1.0f, 0.3f); // Left
	glEnd();

	glFlush();
}

void drawStar(){

	glColor3f(1.0f, 0.0f, 0.0f);

	// Draw a triangle in a fixed position on the gl context
	// Draw the tips of each triangle individually
	glLoadIdentity();
	glBegin(GL_TRIANGLES);
	    glVertex3f(-0.6f, 0.77f, 0.0f);
	    glVertex3f(-0.42f, 0.77f, 0.0f);
	    glVertex3f(-0.58f, 0.68f, 0.0f);
	    //second triangle top triangle
	    glVertex3f(-0.64f, 1.0f, 0.0f);
	    glVertex3f(-0.68f, 0.77f, 0.0f);
	    glVertex3f(-0.60f, 0.77f, 0.0f);
	    //3rd Triangle
	    glVertex3f(-0.68f, 0.77f, 0.0f);
	    glVertex3f(-0.7f, 0.68f, 0.0f);
	    glVertex3f(-0.86f, 0.77f, 0.0f);
	    //4th Triangle
	    glVertex3f(-0.64f, 0.63f, 0.0f);
	    glVertex3f(-0.7f, 0.68f, 0.0f);
	    glVertex3f(-0.82f, 0.43f, 0.0f);
	    //5th Triangle
	    glVertex3f(-0.64f, 0.63f, 0.0f);
	    glVertex3f(-0.58f, 0.68f, 0.0f);\
	    glVertex3f(-0.51f, 0.43f, 0.0f);
    glEnd();

    // Draw a polygon to fill the center of the star.
	glBegin(GL_POLYGON);
		glVertex3f(-0.60f, 0.77f, 0.0f);
		glVertex3f(-0.68f, 0.77f, 0.0f);
		glVertex3f(-0.7f, 0.68f, 0.0f);
		glVertex3f(-0.64f, 0.63f, 0.0f);
		glVertex3f(-0.58f, 0.68f, 0.0f);
	glEnd();
	glFlush();
}

void drawHeptagon(){

	// Draw a Heptagon on the unit plane. Scale it down and move it to one side
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Move the heptagon .5 units to the right
	glTranslatef(0.5f, 0.0f, 0.0f);
	// scale the helptagon to 30% size
	glScalef(0.3f,0.3f,0.3f);
	glBegin(GL_POLYGON);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f); // top
		glVertex2f(0.8f, 0.6f); // Right
		glVertex2f(1.0f, -0.2f); // Hept outer left
		glVertex2f(0.48f, -1.f); // Bottom right
		glVertex2f(-0.48f, -1.0f); // Bottom Left
		glVertex2f(-1.0f, -0.2f); // Hept outer left
		glVertex2f(-0.8f, 0.6f); // Left
	glEnd();

	glFlush();
}

void displayGraphics(){

	// Set up the OpenGL context
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the 3 individual shapes
	drawPentagon();
	drawHeptagon();
	drawStar();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutCreateWindow("Activity 2");
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutDisplayFunc(displayGraphics);
	glutMainLoop();
	return 0;
}
