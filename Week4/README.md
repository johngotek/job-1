# Session 4 Adding movement
![Green Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/green.png?raw=true "You should complete the following steps as a minimum in order to keep up with the tutorial")

## Keyboard input
We are now going to add movement to the hovercraft. In this tutorial we will use the keyboard arrow buttons **up** (forward), **down** (backward), **left** and **right**, **Page Up** (up) and **Page Down** (down). It is equally possible to use other buttons or other methods of input. 

In StartGame.cpp you may have noticed the 

```C++
void keyInput(unsigned char key, int x, int y)
```

This method is linked to glut via:

```C++
glutKeyboardFunc(keyInput);
```

When we are looking into how to move the hovercraft with the keys, this seems like a promising place to start. However if we play with this function and debug it we find out that it only reacts once, when a key is pressed. We would like our Hovercraft to move when the button is being held down, but within this function, there is no way to tell if a key is being held down and for how long. 

Luckily glut also has a:

```C++
void glutKeyboardUpFunc(unsigned char key, int x, int y)
```
So we can use these two functions in combination to record whether a particular button is being pressed. If we introduce a boolean variable, say up for the up key, we can set this to true if it is being pressed in **glutKeyboardFunc** and to false if it is being released in **glutKeyboardUpFunc**. This will then mean that up = true if and only if the up button is currently being pressed.

Next we need to write the functions for keys being pressed and released. However it turns out that glutKeyboardFunc and glutKeyboardUpFunc do not react when the arrow keys and page up/down are being pressed. Instead there are different functions for special keys, that work in a similar way:
```C++
glutSpecialFunc(...);
glutSpecialUpFunc(...);
```

## Generic keyboard input
We would like to be able to react to any key being held, not just a few keys that we introduce booleans for. 

Fortunately a standard data structure exists that allows us to store key, value pairs. We can use the key being pressed as a key and the value can be a boolean indicating whether the key is being pressed. We would need 2 separate variables, one for the normal keys and one for the special keys. As we only need one instance for all game objects, we can declare them as static.

*GameObject.h*
```C++
class GameObject {
private:
...
public:
	static std::map <int, bool> specialKeys;
	static std::map <char, bool> keys;
...
```

We can now set these in StartGame.cpp, as they are public and static we can access them there as well. If you get an error about unresolved externals you will just need to re-iterate the declaration including the class. We can now add functions to save the value for the keys that are being pressed. As we will only use them as input to the glut functions we will use lambda functions. Please note that if a value is not set on the map the value defaults to false (for booleans).

*GameObject.cpp*
```C++

std::map <int, bool> GameObject::specialKeys;
std::map <char, bool> GameObject::keys;

//Lambda functions to link our code to glut's keydown and keyup. Our function deals with both regular and special keys in one.
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

glutSpecialUpFunc([] (int key, int x, int y) {
	GameObject::specialKeys[key] = false;
});
```

## Movement in Hovercraft - delta time

Now we want to work on our update. However if we look in StartGame.cpp we see that update is never being called, so of course it isn't going to do anything right now. First we need to add the call to StartGame.cpp. A good place to do this is in the function that is called when the game is idle, so **void idle()**. Here we will want to call update for all gameobjects. First we need to calculate delta time. As glut has a function to indicate the amount of time elapsed in the game in miliseconds, we will introduce two variables.


*StartGame.cpp*
```C++
...
int oldTimeSinceStart;
int newTimeSinceStart;
...
```

As the times are in milliseconds, if the game is running really fast (1000 fps or more) it is possible for deltaTime to be 0. We want to *avoid that at all cost*, as this is going to lead to divisions by 0 and create a lot of problems. So if our deltatime == 0 we want to pause for 1ms and recalculate. Please note that for mac or linux the sleep command will be different.

Now we call also call all updates for all gameobjects.

*StartGame.cpp*
```C++
...
//Update the game state.
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
	
	//ask glut for a redraw after all the updates
	glutPostRedisplay();	
}
...	
```

## Movement in Hovercraft - variables
The next step is to move our hovercraft based on which key is being held. This is where we will finally make use of our **void update()**. What we want to do with our hovercraft is to turn it with the left/right keys and pitch it up/down with page up / page down. The up and down arrow keys will act as forward and backwards in the direction we are facing.
We will be applying a rotation in two directions. It would be possible to do this with a rotation matrix. However we can also just apply one rotation followed by the other one and use the glm::rotate. However the order of the rotations is very important. If when you try to do this things "go strange" and go out of sync with what you are expecting, it is most likely to do with the order of the rotations. Rotations when coded should be in reverse order.

To be able to do this we introduce a number of variables: a **rotationAngle** and **pitchAngle** at which the hovercraft is currently rotated. As **glm::rotate** takes a vector we will also record a **heading** vector and for reference the original **startingHeading** is also recorded.

As we would later on like to be able to follow any game object with our camera we will declare the heading at the level of GameObject.h

*GameObject.h*
```C++
...
glm::vec3 heading = glm::vec3(0.0, 0.0, -1.0);  ///<Current heading (direction) the GameObject is facing.
...
```

*Hovercraft.h*
```C++
glm::vec3 startingHeading = heading; ///<Starting facing direction of the Hovercraft.
float rotationAngle, pitchAngle = 0.0; ///<Rotation and pitch angles for our Hovercraft
```

## Movement in Hovercraft - update

Now we are going to work on our update. First we need to work out how far we need to move the Hovercraft, both in the forward/backward direction as well as in terms of turning angle. Let's define some constants to make our life easier in Hovercraft.h. We are also going to need to include GLM rotate.


*Hovercraft.h*
```C++
...
#include<glm/gtx/rotate_vector.hpp>
#define TURNING_SPEED 90.0 //<turning speed in degrees per second
#define MOVE_SPEED 40.0 //<move speed in coordinate units per second
...
```

Now we can calculate the amount we should move and turn each time, while the relevant buttons are held. *Please note* that we need to bring in deltaTime and convert it to seconds so that our contsnatds do what we expect.

*Please note:* as it is possible for multiple keys to be pressed at the same time, we will have to do a set of if statements, rather than a switch or even if-else.


*Hovercraft.cpp*
```C++
...
void Hovercraft::update(int deltaTime)
{
	float moveStep = MOVE_SPEED * (deltaTime /1000.0); //movement speed in units per second * deltaTime in sec = moveStep
	float turningSpeed = TURNING_SPEED *  (deltaTime / 1000.0); //turning speed (degrees/sec) * deltaTime in sec = turning speed over delta time
...
```


We will now check which keys have being held. *Remember,* as it is possible for multiple keys to be pressed at the same time, we will have to do a set of if statements, rather than a switch or even if-else statements.


*Hovercraft.cpp*
```C++
...
	if (specialKeys[GLUT_KEY_DOWN]) {
		this->position -= this->heading * moveStep;
	}

	if (specialKeys[GLUT_KEY_UP]) {
		this->position += this->heading * moveStep;
	}

	if (specialKeys[GLUT_KEY_PAGE_UP]) {
		this->pitchAngle += turningSpeed; //in degrees not radians
	}

	if (specialKeys[GLUT_KEY_PAGE_DOWN]) {
		this->pitchAngle -= turningSpeed; //in degrees not radians
	}

	if (specialKeys[GLUT_KEY_LEFT]) {
		this->rotationAngle += turningSpeed; //in degrees not radians
	}

	if (specialKeys[GLUT_KEY_RIGHT]) {
		this->rotationAngle -= turningSpeed; //in degrees not radians
	}

}
...
```

Finally we need to actually make the rotation. We could just rotate by the turningspeed in the direction we are rotating. However when we draw the final scene we will need the total rotation and pitch angles.

In order to make sure that the heading definitely matches what we draw an easy solution is to use the same full rotation angles. To do that we use the starting heading.

*Hovercraft.cpp*
```C++
...
	this->heading = glm::rotate(this->startingHeading, glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));
	this->heading = glm::rotate(this->heading, glm::radians(pitchAngle), glm::vec3(0.0, 0.0, 1.0));
}
...
```

## Movement in Hovercraft - override drawScene() to draw everying at the correct angles

Go ahead and try what we have made so far. You will notice that the Hovercraft moves but does not rotate, or at least you can't see it! We need a slightly different drawScene to the other GameObjects, so we will override it. To do this simply add the drawScene to both Hovercraft.cpp and Hovercraft.h. If you forget to add it to the header, visual studio will remind you and offer to it for you as we have seen before!

So in the drawScene we will first again move the object in place with a translate and then rotate by pitchAngle and rotationAngle. Due to the way that OpenGL works we will now do pitchAngle first and than rotationAngle, which means that it will be done in the same order as in the update.

*Hovercraft.cpp*
```C++
...
void Hovercraft::drawScene() {
	glPushMatrix();
		glTranslatef(this->position.x, this->position.y, this->position.z);

		//rotate openGL object
		glRotatef(pitchAngle, 0.0, 0.0, 1.0);
		glRotatef(rotationAngle, 0.0, 1.0, 0.0);
		glCallList(this->base); // Draw Hovercraft.
	glPopMatrix(); // End draw Hovercraft.
}
...
```

![Amber Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/amber.png?raw=true "You are encouraged to complete the following steps to enhance your learning")
## Game Engine

We now have nearly made a basic game engine. However everything is held together in StartGame.cpp and this contains some code to create and manage a list of GameObjects. Of course this is one way to do it, but it would be nice if this management would be folded into one of the classes, to simplify making different games.

To acheive this we can create a GameEngine class. When we try and do this we realise that actually we do not really want to be able to have several instances of the GameObject. While there are various ways to ensure this, and it would also be nice to introduce more layers of abstraction to allow for example swapping between OpenGL and directX. However that is beyond the scope of this tutorial.

For now we will create a GameEngine class that we use to store the functions that every game would need in it's StartGame.cpp. I have decided to make them static, so that they can directly be called from the main.

We will include the GameObject and <vector> and introduce static variables oldTimeSinceStart, newTimeSinceStart, a vector of GameObjects, an int to store the current displaylist base, and the function that updates the game when idle.



*GameEngine.h*
```C++
...
#include "GameObject.h"
#include <vector>

class GameEngine {
private:
	static std::vector<GameObject*> gameobjects; ///<Vector of (pointers to) game objects: all the objects in the game.
	static void updateGame(); ///<Function that updates the game state
	static int oldTimeSinceStart; ///<The old time since the start of the game (from previous frame) for delta time calculation.
	static int newTimeSinceStart; ///<The time since the start of the game for delta time calculation.
	static unsigned int base; // Generate display list base. 	
...
```

Next we public expose a function to add a gameobject, to initialise and to start the engine and to cleanup the memory used. Don't forget to re-iterate these declarations in the cpp file, otherwise you will get error about unresolved externals!


*GameEngine.h*
```C++
...
public:
	static void initEngine(int argc, char **argv, const char* windowTitle = "New Game", bool debugMode = false, int width = 500, int height = 500); ///<Initialises the engine, initilising glut and OpenGL.
	static void addGameObject(GameObject* gameobject); ///<Adds a GameObject to the game.
	static void startEngine(); ///<Start the engine (and the main game loop).
	static void cleanup(); ///<Function to clean up and delete any pointers and anything else when the game exists to prevent memory leaks.
...
```

The initEngine contains the OpenGL setup code such as window title and size etc.
*GameEngine.cpp*
```C++
...

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
	
	//glut events
	glutDisplayFunc([]() { 
		//Prepare for drawing all objects.
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Call drawscene for all gameobjects.
		for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
				gameobjects[i]->drawScene();
		}
		glutSwapBuffers(); //Finish Draw Scene.
	});
...
```


In the initEngine, we also link GLUT events for the displayfunc, reshape, and keydown functions. In the displayfunc we will call all the drawscenes. 

To keep the code as clean as possible we have introduced a separate displayFunc and reshapeFunc, but for the key actions we use lambda functions.

*GameEngine.cpp*
```C++
...
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
...
```

Adding a game object is a matter of putting it on the list, calling setupDrawing and start. We also call glGenLists to get the next displaylist base.
*GameEngine.cpp*
```C++
...
//Add a new gameobject to the scene.
void GameEngine::addGameObject(GameObject* gameobject) {
	gameobjects.push_back(gameobject);
	base = glGenLists(1);
	base = gameobject->setupDrawing(base);
	gameobject->start();
}
...
```

UpdateGame is what we used to have in the idle.

*GameEngine.cpp*
```C++
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


	//Run update for all game objects.
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		gameobjects[i]->update(deltaTime);
	}
	
	//Done so indicate that it's time to redraw.
	glutPostRedisplay();
}
...
```

To start the engine we need to start the glut main gameloop.

*GameEngine.cpp*
```C++
...
void GameEngine::startEngine() {
	//Explain main interaction with scene.
	std::cout << "Press escape to exit." << std::endl;

	//Start game loop.
	glutMainLoop();
}
...
```


The cleanup function, which we should call before exit. Here we delete pointer objects for when we exit, to prevent memory leaks.
*GameEngine.cpp*
```C++
...
void GameEngine::cleanup() {
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it) {
		delete *it;
	}
	gameobjects.clear();	
}
...
```


Now our startGame.cpp can become very simple indeed:

*StartGame.cpp*
```C++
#include "GameEngine/GameEngine.h"
#include "GameEngine/Hovercraft.h"
#include "GameEngine/Obstacle.h"

int main(int argc, char **argv) {

	//Initialise the engine.
	GameEngine::initEngine(argc, argv, "Hovercraft tutorial",true);

	//Adding an Obstacle.
	GameEngine::addGameObject(new Obstacle(glm::vec3(0.0, 0.0, -25.0), { 0.55f, 0.27f, 0.07f }));
	
	//Adding an Obstacle with different colour.
	GameEngine::addGameObject(new Obstacle(glm::vec3(5.0, 5.0, -25.0)) );

	//Add a movable hovercraft.
	GameEngine::addGameObject(new Hovercraft(glm::vec3(0.0, -5.0, -25.0)), false);

	//Start the game engine.
	GameEngine::startEngine();

	return 0;
}
```

![Red Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/red.png?raw=true "The following are advanced steps you can complete when you are done with everything else.")
## Removing objects
Within games it is not unusual to want to remove objects, for example to remove Obstacle when colliding with them. For this red exercise, implement a system to delete objects during the game. 

**Hint:** you cannot really destroy the object from within itself as you'll have a dangling pointer on the list of gameobjects. We want to avoid circular dependancies so we prefer not to have to access the list of game objects from within an object. One solution is to work with a flag (boolan) and periodically check in the GameEngine which objects need removing.

## Camera follow
Implment the ability (optional) to have the camera follow an object. You can use gluLookAt and make use of the object's position and/or heading.

## Debug mode
While you are building your game, having a debug mode is very usfeul. Go ahead an implement a debug mode.
When the debug mode is on, objects could draw additional information (e.g. the hovercraft could draw it's heading vector).
