/////////////////////////////////////////////////////////////////////////////////////////////        
/// @class GameObject
/// @brief This class is the base class for all objects in the scene. It is a pure virtual class and you cannot directly instantiate it but you can create classes that extend it and those can all be added to the scene.
///
/// @author Maurice Hendrix
///////////////////////////////////////////////////////////////////////////////////////////// 
#pragma once
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

# include <iostream>
# include <glm/glm.hpp> 
class GameObject
{
protected:
	unsigned int base;
public:
	glm::vec3 position;
	
	//////////////////////////////////
	/// Constructor that sets the position.
	///
	/// @param[in] pos Position in scene in 3 axis
	//////////////////////////////////	
	GameObject(glm::vec3 position);
	~GameObject();
	
	//////////////////////////////////
	/// Performs the drawing setup for this game object. Can optionally be overwritten by inherited classes (as it is a virtual function).
	/// @note Inspired by Sumantha's ship example.
	///
	/// @param[in] listBase base index for the display list
	/// @param[in] *qobject A pointer to a quadric object
	/// @returns The base index for the display list	
	//////////////////////////////////	
	virtual unsigned int setupDrawing(unsigned int listBase);
	
	//////////////////////////////////
	/// Function to perform this object's part of the redraw work. The default version in GameObject will put the object in the right place and redraw it. This can be overridden.
	/// @note Inspired by Sumantha's ship example.
	//////////////////////////////////
	virtual void drawScene(); 

	/* Every GameObject must implement the following pure virtual function or otherwise cause a compile error.*/
	virtual void start() = 0; ///<Pure virtual start function, run once when the engine starts.  Must override in inherited classes.
	virtual void update(int deltaTime) = 0; ///<Pure virtual update function, run periodicly to update the game state. Must override in inherited classes.
};

