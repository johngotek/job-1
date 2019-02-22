/////////////////////////////////////////////////////////////////////////////////////////////        
/// @class Obstacle
/// @brief A very simple object that represents an Obstacle
///
/// @author Maurice Hendrix
///////////////////////////////////////////////////////////////////////////////////////////// 
#pragma once
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////        
/// @class Colour
/// @brief POD class (Plain old data). Holds a red, green and blue value.
///////////////////////////////////////////////////////////////////////////////////////////// 
struct Colour {
	float r, g, b;
};

class Obstacle : public GameObject
{
public:
	//////////////////////////////////
	/// Constructor that sets the position and colour.
	///
	/// @param[in] pos Position in scene in 3 axis
	/// @param[in] col Colour of the object in r,g,b
	//////////////////////////////////
	Obstacle(glm::vec3 pos, Colour col = { 0,0,1 });
	~Obstacle();///<Default destructor.

	Colour colour;///<Current colour of the object.

	/* Overridden from the GameObject class.*/
	unsigned int setupDrawing(unsigned int listBase);///<Overridden setup drawing method.
	void start();  ///Overridden start method.
	void update(int deltaTime);  ///<Overridden update method.
	void collides(Collider * other);///<Overridden collides method.
};

