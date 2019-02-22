#pragma once
#include "GameObject.h"
#include<glm/gtx/rotate_vector.hpp>
#define TURNING_SPEED 90.0 //<turning speed in degrees per second
#define MOVE_SPEED 40.0 //<move speed in coordinate units per second

class Hovercraft : public GameObject
{
public:
	Hovercraft(glm::vec3 pos);
	~Hovercraft();

	glm::vec3 startingHeading = heading; ///<Starting facing direction of the Hovercraft.
	float rotationAngle = 0.0; ///<Rotation angle for our Hovercraft
	float pitchAngle = 0.0; ///<Pitch angles for our Hovercraft

	unsigned int setupDrawing(unsigned int listBase);
	void start();
	void update(int deltaTime);
	void drawScene();
};