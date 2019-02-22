# Session 3 Multiple game objects & inheritance

![Green Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/green.png?raw=true "You should complete the following steps as a minimum in order to keep up with the tutorial")
## Hovercraft objects

Last session we transferred the Obstacle into its own class. We managed to make the class self contained, and to give it a position and colour, so that you can easily create multiple Obstacles in different positions.

The next step is to do a similar thing with the Hovercraft. How would we go about doing this? Think about this for a minute...

No doubt you will be tempted to copy the code from the Obstacle class and paste it into the Hovercraft class (both for a header file Hovercraft.h and the cpp file Hovercraft.cpp). 

This seems like a quick an easy way forward but it is a *very bad idea indeed!* There are several reasons why this is such a bad idea:

- It creates 2 copies of various pieces of code. These two pieces do the same thing. However it is not easy for another programmer to be sure that they do the same.
- If the code contains any errors it is very difficult, even if you have written the code, to remember to fix all copies. This often leads to bugs that you thought were fixed re-appearing. 
- It creates a bigger code base that will be harder to understand than if both classes could use the same code without the need for a copy.
- We will end up with two things (an Obstacle and a Hovercraft) which are conceptually similar: they are both objects in the game scene. However as far as the compiler is concerned these two classes are entirely different and have nothing to do with each other. This also means that you can not easily make a list (vector) of items in the game scene. It is possible to use a vector of objects, but then we would not be able to use any of the methods we have declared on the items in the vector.

Fortunately it is possible to almost entirely remove the need for copying code by using inheritance. In our case we will introduce a third class, which we will call gameObject. This class will contain everything that is shared between the Obstacle and our Hovercraft object. This means it will have all variable and function declarations for variables and functions that are in both objects. It will also contain the code, or function implementations, where possible.

The result is that the Obstacle and the Hovercraft classes will contain just what is different and end up being much smaller than they otherwise would, we can now create a vector of GameObject items and we can use all shared variables and methods (that are public).

The first step is to create a new class via the add class wizard called GameObject . This will create GameObject.h and GameObject.cpp. Of course this class is empty. The next step would be to make our Obstacle inherit from GameObject. The way to think about is is that our Obstacle is a type of GameObject, just like our Hovercraft will be. Or for example if we were building a shooter game we may have a weapon class and then a gun and a knife which are both weapons.

To do this change the class declaration in in Obstacle.h to include the parent class:
```C++
#include "GameObject.h"

class Obstacle : public GameObject
{
...
};
```

Now we can move some of the declarations into the GameObject class.

The first thing we will move is the OpenGL include, as this will clearly be needed in both classes. We remove these from Obstacle.h as the include of Gameobject.h will also include the OpenGL includes.

```C++
#pragma once
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
  #define OPENGL_INCLUDES
  #  include <GL/glew.h>
  #  include <GL/freeglut.h>
  #  include <GL/glext.h>
  #pragma comment(lib, "glew32.lib") 
#endif 

#include <iostream>
#include "Collider.h" //gameobjects can have a collider

class GameObject{
...
}
```

Now we can move the position and the base (for the drawing list) variable from Obstacle.h to Gameobject.h as all GameObjects will need a position. To this end, remove the declaration of the variable position from Obstacle.h. It is very important that you remove this, to prevent unexpected behaviour. In GameObject.h now add the position declaration.

When you move the declaration, think about the access level. If you declare it as **private**, it would mean that the Obstacle would not have access to it. You can declare it as **protected**. This would mean that the variable would be protected from outside access, but that classes that extend this class can have access to it. Alternatively you may want the position to be something that you can access from the outside in which case we can make it **public**. In our case we will need access to the position, much later on when following objects with the camera.




```C++
...
class GameObject
{
protected:
	unsigned int base;
public:
	glm::vec3 position;
	GameObject();
	~GameObject();

};
...
}
```

We notice that the add new class wizard has added a default constructor **GameObject();**. As all game objects will have a position we would like this to be stored at gameobject level. So we change the constructor declaration and add the implementation:

*GameObject.h*
```C++
...
	GameObject(glm::vec3 position);
...
```

*GameObject.cpp*
```C++
...
GameObject::GameObject(glm::vec3 pos){
	this->position = pos;
}
...
```

Now we can make use of this constructor in the asteroid class, so we delegate dealing with saving the position to the GameObject class. The following constructor also take in a colour for being able to draw the Obstacle in the correct colour. 

If you weren't making any chances (like adding in the colour) you would actually not even need to declare the constructor at this level. Note the use of the default parameter in the Obstacle constructor. (If we don't give a value for col when we create an instance, the default is used.)

*Obstacle.h*
```C++
...
Obstacle(glm::vec3 pos, Colour col = { 0,0,1 });
...
```

*Obstacle.cpp*
```C++
...
Obstacle::Obstacle(glm::vec3 pos, Colour col) : GameObject(pos) {
	this->colour = col;
}
...
```

The next functions that all GameObjects will have are the **setupDrawing**, **drawScene**, **start** and **update**. However, **drawScene** can be implemented at this level, but it is not possible to implement these others at the level of the GameObject, as for example with drawScene we don't know how to draw the object yet in the GameObject class.

One possibility would be not to move these functions into the GameObject. However if you did this, and you stored all GameObjects in a vector you would not be able to use these functions, so you couldn't walk through a list of GameObjects and draw them all.

A really nice possibility to deal with this is to declare the functions as virtual. This means that all classes that extend GameObject have to provide an implementation. It also prevents the code from ever creating an instance of GameObject, which suits us as we only want concrete things we can draw. The GameObject class would now be used as an abstract class.

To be able to do this we actually leave the code in the Obstacle class alone and add the following declarations to gameObject.h:

*GameObject.h*
```C++
...
	virtual unsigned int setupDrawing(unsigned int listBase);
	virtual void drawScene(); ///Function to perform this object's part of the redraw work. The default version in GameObject will put the object in the right place and redraw it. This can be overridden.
	
    /* Every GameObject must implement the following pure virtual function or otherwise cause a compile error.*/
	virtual void start()=0; ///<Pure virtual start function, run once when the engine starts.  Must override in inherited classes.
	virtual void update(int deltaTime)=0; ///<Pure virtual update function, run periodicity to update the game state. Must override in inherited classes.
...
```

![Amber Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/amber.png?raw=true "You are encouraged to complete the following steps to enhance your learning")
## Hovercraft class

Now create a Hovercraft class that inherits from GameObject (just like Obstacle). In the main program (StartGame.cpp) add a variable of type Hovercraft and make sure to call the relevant draw and update methods.

![Red Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/red.png?raw=true "The following are advanced steps you can complete when you are done with everything else.")
## Vector

Until now we have created the Obstacle and the Hovercraft as objects and made sure to call the drawScene, update etc. for each object. As the amount of objects in our scene grows this will become unsustainable. For example the drawScene method may become very long:

*GameObject.h*
```C++
...
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Position the objects for viewing.
	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	firstObstacle.drawScene();
	secondObstacle.drawScene();
	thirdObstacle.drawScene();	
	.....
	sixhundredthObstacle.drawScene();
	sixhundredAndFirstObstacle.drawScene();
	....
	....
}
...
```

Can you think of a better more sustainable way of doing this and making use of the inheritance between GameObject and Obstacle/Hovercraft? Hint: you may want to use a vector.
