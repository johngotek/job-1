#include "Obstacle.h"

Obstacle::Obstacle(glm::vec3 position, Colour colour): GameObject (position)
{
	this->colour = colour;
}

Obstacle::~Obstacle(){
}

unsigned int  Obstacle::setupDrawing(unsigned int listBase) {
	this->base = ++listBase;
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(this->colour.r, this->colour.g, this->colour.b);
			glRotatef(-45, 1, 0, 0);
			glutSolidCone(0.5, 0.75, 30, 30);
		glPopMatrix();
	glEndList();
	return this->base;
}

void Obstacle::start()
{
}

void Obstacle::update(int deltaTime)
{
}