/////////////////////////////////////////////////////////////////////////////////////////////        
/// GameEngine.cpp
///
/// Implementation for a simple example game engine.
///
/// @author Maurice Hendrix
///////////////////////////////////////////////////////////////////////////////////////////// 
#include "GameEngine.h"

bool GameEngine::debugMode;
std::vector<GameObject*> GameEngine::gameobjects; //The definition of static variables needs to be repeated in the cpp file.
GameObject* GameEngine::cameraFollow; 
int GameEngine::oldTimeSinceStart;
int GameEngine::newTimeSinceStart;
std::map <int, bool> GameObject::specialKeys;
std::map <char, bool> GameObject::keys;
unsigned int GameEngine::base; // Generate display list base. 

void GameEngine::initEngine(int argc, char **argv, const char* windowTitle, bool debug, int width, int height){
	GameEngine::debugMode = debug;
	GameObject::debugMode = GameEngine::debugMode;
	//Init glut.
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowTitle);

	glewExperimental = GL_TRUE;
	glewInit();


	//Lambda function defined inside the glutDisplayFunc call to call all relevant drawscenes.
	glutDisplayFunc(displayFunc);

	//Lambda function defined inside glutReshapeFunc to do resize logic.
	glutReshapeFunc(reshapeFunc);

	//Lambda functions to link our code to glut's keydown andkeyup. Our function deals with both regular and special keys in one.
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		GameObject::keys[key] = true;
		//if we press escape, exit the game
		if (key == 27) {
			cleanup();
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

	glutIdleFunc(updateGame);
	
}

void GameEngine::displayFunc() {
	//Prepare for drawing all objects.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//If we have no object to follow just put the cam in a static position.
	if (cameraFollow != NULL) {
		//Hardcoded camera variables for the distance bewteen camera and object and y axis distance between camera and object.
		float distance = 8;
		float yAm = 2;

		gluLookAt(cameraFollow->position.x - (cameraFollow->heading.x * distance), cameraFollow->position.y - (cameraFollow->heading.y * distance) + yAm, cameraFollow->position.z - (cameraFollow->heading.z * distance),
			cameraFollow->position.x, cameraFollow->position.y, cameraFollow->position.z,
			0.0, 1.0, 0.0);
	}else {
		gluLookAt(0.0, 10.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	//Call drawscene for all gameobjects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->drawScene();
	}
	glutSwapBuffers(); //Finish Draw Scene.
}

void GameEngine::reshapeFunc(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
};

void GameEngine::startEngine() {
	//Explain main interaction with scene.
	std::cout << "Press escape to exit." << std::endl;

	//Start game loop.
	glutMainLoop();
}

//Add a new gameobject to the scene.
void GameEngine::addGameObject(GameObject* gameobject, bool camFollow) {
	gameobjects.push_back(gameobject);
	if (camFollow) {
		cameraFollow = gameobject;
	}

	base = glGenLists(1);
	base = gameobject->setupDrawing(base);
	gameobject->start();
}

//Update the game state.
void GameEngine::updateGame() {
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

	//Remove any inactive objects first.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		if (!gameobjects[i]->active) {
			delete gameobjects[i];
			gameobjects.erase(gameobjects.begin() + i);
		}
	}

	//Run update for all game objects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}
	
	//Test collisions in a naive way for every pair of gameobjects (note calculations will explode when you increase the amount of gameobjects. Can you think of a better way?
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		for (std::vector<GameObject*>::size_type j =i+1 ; j != gameobjects.size(); j++) {
			if (gameobjects[i]->collider != NULL) {
				if (gameobjects[i]->collider->collidesWith(gameobjects[j]->collider)) {
					gameobjects[i]->collides(gameobjects[j]->collider);
					gameobjects[j]->collides(gameobjects[i]->collider);
				}
			}
		}
	}

	//Done so indicate that it's time to redraw.
	glutPostRedisplay();
}

//Here we delete pointer objects for when we exit, to prevent memory leaks.
void GameEngine::cleanup() {
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it) {
		delete *it;
	}
	gameobjects.clear();	
}