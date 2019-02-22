#include "GameObject.h"



GameObject::GameObject(glm::vec3 position)
{
	this->position = position;
}

GameObject::~GameObject()
{
}


void GameObject::drawScene()
{
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glCallList(this->base);
	glPopMatrix();
}