#ifndef __GRASS_H__
#define __GRASS_H__

#include "GameObject.h"

class Grass: public GameObject{
private:
	glm::vec3 size;
public:

	Grass(const glm::vec3& _pos, const glm::vec3& _size);
	unsigned int setupDrawing(unsigned int listBase);
	void drawScene();
	void start();
	void update(int deltaTime);
};
#endif
