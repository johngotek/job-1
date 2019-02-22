#include "Grass.h"

Grass::Grass(const glm::vec3 & _pos, const glm::vec3 & _size): GameObject(_pos){
	size = _size;
}

unsigned int Grass::setupDrawing(unsigned int listBase) {
	this->base = listBase;
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(0, 1, 0);
			glBegin(GL_QUADS);
				glVertex3f(position.x + size.x, position.y, position.z + size.z);
				glVertex3f(position.x - size.x, position.y, position.z + size.z);
				glVertex3f(position.x - size.x, position.y, position.z - size.z);
				glVertex3f(position.x + size.x, position.y, position.z - size.z);
			glEnd();
		glPopMatrix();
	glEndList();
	return this->base + 1;
}

void Grass::drawScene() {
	glPushMatrix();
		glCallList(this->base); // Draw grass.
	glPopMatrix(); // End draw grass.

}

void Grass::start() {
}

void Grass::update(int deltaTime) {
}
