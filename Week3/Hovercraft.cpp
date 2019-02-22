#include "Hovercraft.h"

Hovercraft::Hovercraft(glm::vec3 position) : GameObject(position)
{
}

Hovercraft::~Hovercraft()
{
}

unsigned int Hovercraft::setupDrawing(unsigned int listBase)
{
	this->base = ++listBase;
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(-2, 0, 2);
			glRotatef(-90, 0, 1, 0);
			glutSolidCone(0.5, 0.75, 30, 30);
			glPushMatrix();
				glTranslatef(0, 0, -0.4);
				glColor3f(1, 1, 1);
				glutSolidCube(0.8);
			glPopMatrix();
		glPopMatrix();
	glEndList();
	return this->base;
}

void Hovercraft::start()
{
}

void Hovercraft::update(int deltaTime)
{
}