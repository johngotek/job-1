/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 

#include "Obstacle.h";
#include "Hovercraft.h";

Grass grassField = Grass(glm::vec3(0, 0, 0), glm::vec3(5, 0, 5));
Obstacle firstObstacle = Obstacle(glm::vec3(0.0, 0.0, -2.0), {0,0,255});
Obstacle secondObstacle= Obstacle(glm::vec3(4.0, 0.0, 4.0), {0,255,0});
Hovercraft hovercraft = Hovercraft(glm::vec3(-2, 0, 2));


// Drawing routine.
void drawScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Position the objects for viewing.
	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	grassField.drawScene();
	firstObstacle.drawScene();
	secondObstacle.drawScene();
	hovercraft.drawScene();

	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	unsigned int base = glGenLists(4); // Generate display list base. 
	baase = grassField.setupDrawing(base);
	base = firstObstacle.setupDrawing(base);
	base = secondObstacle.setupDrawing(base);
	base = Hhovercraft.setupDrawing(base);

	grassField.start();
	firstObstacle.start();
	secondObstacle.start();
	hovercraft.start();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void idle()
{
	glutPostRedisplay();
}
// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Space Program");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutIdleFunc(idle);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
