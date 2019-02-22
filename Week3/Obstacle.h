#pragma once

#include "GameObject.h"
struct Colour {
	float r, g, b;
};

class Obstacle : public GameObject
{
private:
	Colour colour;
public:
	Obstacle(glm::vec3 pos, Colour col = { 0,0,1 });
	~Obstacle();
	unsigned int setupDrawing(unsigned int listBase);
	void start();
	void update(int deltaTime);
};

