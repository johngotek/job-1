/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 

#include <vector>
#include "Obstacle.h";
#include "Hovercraft.h";
std::map <int, bool> GameObject::specialKeys;
std::map <char, bool> GameObject::keys;

std::vector<GameObject*> gameobjects; //A list of game objects

int oldTimeSinceStart=0;
int newTimeSinceStart=0;


// Drawing routine.
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Position the objects for viewing.
	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->drawScene();
	}

	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	unsigned int base = glGenLists(gameobjects.size() +1); // Generate display list base. 

	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		base = gameobjects[i]->setupDrawing(base);
	}

	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->start();
	}
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


void idle() {
	//Calculate delta time (in mili seconds).
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = newTimeSinceStart - oldTimeSinceStart;

	//If the last frame was rendered less than 1 ms ago, the detalaTime will be 0 ms. This causes problems in calculations, so sleep for 1ms to adjust.
	if (deltaTime == 0) {
		Sleep(1);
		newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newTimeSinceStart - oldTimeSinceStart;
	}

	//Run update for all game objects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}

	//ask glut for a redraw after allthe updates
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


	//Lambda functions to link our code to glut's keydown andkeyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = true;
		//if we press escape, exit the game
		if (key == 27) {
			exit(0);
		}
	});

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = false;
	});

	glutSpecialFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = true;
	});

	glutSpecialUpFunc([](int key, int x, int y) {
		GameObject::specialKeys[key] = false;
	});

	// adding a bunch of new game objects to the list

	GameEngine::addGameObject(new Grass(glm::vec3(0, 0, 0), glm::vec3(5, 0, 5)) );
	GameEngine::addGameObject(new Obstacle(glm::vec3(0.0, 0.0, -2.0), { 0.55f, 0.27f, 0.07f }));
	GameEngine::addGameObject(new Obstacle(glm::vec3(4.0, 0.0, 4.0)) );
	GameEngine::addGameObject(new Hovercraft(glm::vec3(-2, 0, 2)), false);

	glutIdleFunc(idle);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
