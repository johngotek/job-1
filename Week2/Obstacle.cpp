#include "Obstacle.h"

Obstacle::Obstacle(glm::vec3 position)
{
	this->position = position;
}


Obstacle::~Obstacle()
{
}

unsigned int  Obstacle::setupDrawing(unsigned int listBase) {
	this->base = ++listBase;
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(0.55, 0.27, 0.07);
			glRotatef(-45, 1, 0, 0);
			glutSolidCone(0.5, 0.75, 30, 30);
		glPopMatrix();
	glEndList();
	return this->base;
}

void Obstacle::drawScene()
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glCallList(this->base);
	glPopMatrix();
}

void Obstacle::start()
{
}

void Obstacle::update(int deltaTime)
{
}
