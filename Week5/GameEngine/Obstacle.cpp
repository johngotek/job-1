#include "Obstacle.h"
#include "CubeCollider.h"

Obstacle::Obstacle(glm::vec3 pos, Colour col) : GameObject(pos) {
	this->colour = col;
	Obstacle::collider = new CubeCollider(&this->position,2, 2, 2);
}

Obstacle::~Obstacle(){
	delete Obstacle::collider;
	Obstacle::collider = NULL;
}

unsigned int  Obstacle::setupDrawing(unsigned int listBase) {
	this->base = listBase;
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(colour.r, colour.g, colour.b);
			glRotatef(-45, 1, 0, 0);
			glutSolidCone(0.5, 0.75, 30, 30);
		glPopMatrix();
	glEndList();

	return this->base +1;
}

void Obstacle::start(){
}

void Obstacle::update(int deltaTime){
}

void Obstacle::collides(Collider* other) {
	if (debugMode) {
		std::cout << "Obstacle collides, Obstacle will be removed!" << std::endl;
	}
	this->active = false;
}