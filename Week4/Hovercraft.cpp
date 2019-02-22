#include "Hovercraft.h"

SpaceShip::Hovercraft(glm::vec3 pos) :GameObject(pos) {
}

SpaceShip::~Hovercraft() {
}

unsigned int SpaceShip::setupDrawing(unsigned int listBase) {
	this->base = listBase;
	// Draw ship.
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

	return this->base + 1;
}

void Hovercraft::drawScene() {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);

	//rotate openGL object
	glRotatef(pitchAngle, 0.0, 0.0, 1.0);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);
	glCallList(this->base); // Draw Hovercraft.
	glPopMatrix(); // End draw Hovercraft.

}

void Hovercraft::start() {
}

void SpaceShip::update(int deltaTime) {
	float moveStep = MOVE_SPEED * (deltaTime / 1000.0); //movement speed in units per second * deltaTime in sec = moveStep
	float turningSpeed = TURNING_SPEED *  (deltaTime / 1000.0); //turning speed (degrees/sec) * deltaTime in sec = turning speed over delta time

	if (specialKeys[GLUT_KEY_DOWN]) {
		this->position -= this->heading * moveStep;
	}
	if (specialKeys[GLUT_KEY_PAGE_UP]) {
		this->pitchAngle += turningSpeed; //in degrees not radians
	}
	if (specialKeys[GLUT_KEY_PAGE_DOWN]) {
		this->pitchAngle -= turningSpeed; //in degrees not radians
	}

	if (specialKeys[GLUT_KEY_LEFT]) {
		this->rotationAngle += turningSpeed; //in degrees not radians
	}
	if (specialKeys[GLUT_KEY_UP]) {
		this->position += this->heading * moveStep;
	}
	if (specialKeys[GLUT_KEY_RIGHT]) {
		this->rotationAngle -= turningSpeed; //in degrees not radians
	}

	//rotate the heading
	//In the drawscene we will need to rotate by the full rotation since the start
	//Therfore we start heard from the original startingheading, so that we can use the same variables for the rotations
	this->heading = glm::rotate(this->startingHeading, glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));
	this->heading = glm::rotate(this->heading, glm::radians(pitchAngle), glm::vec3(0.0, 0.0, 1.0));
}