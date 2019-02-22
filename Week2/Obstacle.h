#pragma once
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 

# include <glm/glm.hpp> 
class Obstacle
{
private:
	glm::vec3 position;
	unsigned int base;
public:
	Obstacle(glm::vec3 position);
	~Obstacle();
	unsigned int setupDrawing(unsigned int listBase);
	void drawScene(); ///<function to perform this object's part of the redraw work.
	void start();
	void update(int deltaTime);
};

