#include <GL/freeglut.h>

void applyDepthSettings(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void displayGraphics(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
	// Change glRotatef() params to rotate obejct and see other sides
	glRotatef(45.0f, 1.0f, 0.0f, 1.0f);

	// Draw each individual face of the 3d shape
	glBegin(GL_QUADS);

		// Front (red)
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.5f, 1.0f, 1.0f);
		glVertex3f(0.5f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		// Right (green)
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.5f, 1.0f, -1.0f);
		glVertex3f(-0.5f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		// Top (blue)
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.5f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 1.0f, -1.0f);
		glVertex3f(0.5f, 1.0f, -1.0f);

		// Left (Yellow)
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.5f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);


		// Back (Purple)
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-0.5f, 1.0f, -1.0f);

		// Bottom (Cyan)
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height){
	if (height == 0) height =1;
		GLfloat aspect = (GLfloat)width / (GLfloat)height;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
	// Set up glut enviornment
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	// Set initial window resolution
	glutInitWindowSize(1280, 720);
	// Set initial window position
	glutInitWindowPosition(0, 0);
	// Name window
	glutCreateWindow("Activity 3");
	// Set the "drawing" function
	glutDisplayFunc(displayGraphics);
	// Reshape view based on aspect ratio?
	glutReshapeFunc(reshape);
	applyDepthSettings();
	glutMainLoop();
	return 0;
}
