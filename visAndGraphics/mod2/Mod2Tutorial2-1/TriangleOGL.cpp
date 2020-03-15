#include <GL/freeglut.h>

void displayGraphics() {


	// Set up FL modes
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(185.0f, 0.0f, 0.0f, 0.0f);

		// Start drawing.
	glBegin(GL_POLYGON);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.5f, 0.5f);

	glEnd();

	glFlush();

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutCreateWindow("OpenGL Triangle");
	glutInitWindowSize(1280,720);
	glutInitWindowPosition(0,0);
	glutDisplayFunc(displayGraphics);
	glutMainLoop();
	return 0;
}
