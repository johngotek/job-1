#pragma once
#include "GameObject.h"

class Hovercraft :
	public GameObject
{
public:
	Hovercraft(glm::vec3 pos);
	~Hovercraft();

	unsigned int setupDrawing(unsigned int listBase);
	void start();
	void update(int deltaTime);
};

