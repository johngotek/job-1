#pragma once
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 

# include <map>
# include <glm/glm.hpp> 
class GameObject
{
protected:
	unsigned int base;
public:
	static std::map <int, bool> GameObject::specialKeys;
	static std::map <char, bool> GameObject::keys;

	glm::vec3 heading = glm::vec3(0.0, 0.0, -1.0);  ///<Current heading (direction) the GameObject is facing.
	glm::vec3 position;///<Position of the gameobject.

	GameObject(glm::vec3 position);
	~GameObject();
	virtual void drawScene(); ///Function to perform this object's part of the redraw work. 
	virtual unsigned int setupDrawing(unsigned int listBase) = 0; ///<Pure virtual function, to set up the drawing

	/* Every GameObject must implement the following pure virtual function or otherwise cause a compile error.*/
	virtual void start() = 0; ///<Pure virtual start function, run once when the engine starts.  Must override in inherited classes.
	virtual void update(int deltaTime) = 0; ///<Pure virtual update function, run periodicly to update the game state. Must override in inherited classes.
};

