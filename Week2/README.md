# Session 2 Our first classes

![Green Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/green.png?raw=true "You should complete the following steps as a minimum in order to keep up with the tutorial")
## Creating a class for the Obstacle
The rest of this tutorial starts from the static scene we created last week. Over the next few weeks, we will be adding movement and keyboard controls as well as basic collision detection between objects. In the process we will create a very simple game engine using classes and inheritance.

Last week we created a static scene.  However as our game grows larger than just simple examples, this method will become unsustainable. The cpp file will become very large and it will be difficult to find the individual bits in the code. To make it even worse if you want many similar elements you would find yourself copy-pasting bits of code, no doubt finding some bugs and only fixing it in some copy and not others. Ultimately it will become a huge unmanagable mess.

Fortunately there is a better way to deal with the growth of the code and this is to use classes. While initially it will take a bit of effort to set up it will save you a lot of time even over the course of a relatively small game like the one for this module. 
Let us start with the Obstacle, represented by the coloured ball In our hovercraft race game we would like more than just one Obstacle. Obstacles in this will be quite similar. They will differ in position in the scene and there may be some slight visual variations such as the colour.
In order to do this we will introduce a class called Obstacle. We can do this from within visual studio as follows:

Go to project and than to Add Class Wizard.

![Screenshot showing starting add class wizard](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/001.png?raw=true "Start the add class wizard")

Click on Add Class.

![Click on add class](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/002.png?raw=true "Click on add class")

Give the class the name Obstacle. The wizard will propose to create Obstacle.h and Obstacle.cpp this is fine, so click finish and then OK to close the wizard.

By convention a class has two files associated with it, of the same name: the header file (.h files) and the .cpp file. Header files are intended for the class signature. This contains everything a compiler needs to know to use the class. It is also a place where the programmer can look at what is in a class without being distracted by lots of implementation code. That is why imports usually import .h files with only few standard packages as exception. Imports should never import .cpp files! The .cpp file is where the implementation details go.

![Add class wizard](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/003.png?raw=true "Add class wizard")

Now we have an Obstacle class in our project. Of course it does not do anything just yet. The main thing we want the Obstacle object to do is draw the Obstacle. Therfore we are going to create a draw function. Let's call the function void drawScene().

We want to add this to Obstacle.h. We would like to be able to tell the Obstacle to draw from outside the class so we place the method after the keyword *public* but before the closing bracked *}*.
After doing this Obstacle.h should look as follows.

```C++
class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	void drawScene(); 
};
```

This code will run. however you may notice that the drawScene in the editor is underlined. However it is underlined in green, unlike when there is an obvious error (when it would be underlined in red). If we hover with our mouse over it we can see what is going on.

![Warning for missing definition](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/004.png?raw=true "Warning for missing definition")
 
Visual studio actually warns us that we have introduced a void drawScene in the header, but it doesn't exist in the .cpp file. If we had creatd the drawScene in the cpp file first we would get a similar warning. Go ahead and click on *show potential fixes*.

![Show potential fixes](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/005.png?raw=true "Show potential fixes") 

Go right ahead and click on *Create the definition of 'drawScene' in Obstacle.cpp*. We now see a small part of Obstacle.cpp pop up inside a inline window. We can close this window with the cross at the top right. 

![Add definition](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/006.png?raw=true "Add definition") 

If we now you look in Obstacle.cpp we will see that the drawScene has been added, with curly braces for us to write code in.

```C++
#include "Obstacle.h"

Obstacle::Obstacle()
{
}


Obstacle::~Obstacle()
{
}

void Obstacle::drawScene()
{
}
```

The next step will be to move the bit of code that draws the Obstacle from the drawScene in startGame.cpp to that in Obstacle.cpp. Hint: the comments in this case helpfully indicate where each object gets drawn.

Once we have moved this into Obstacle.cpp, we wil see all sorts of red underlined stuff and if we press debug it will not work and we get a host of errors.

**Important: when dealing with error messages, start at the top of the list with the very first error message and READ the message. Try and fix it, put it in google, ask for help if necessary, but DO NOT move on to other errors halfway down the list. Once you have fixed the first error, recompile and repeat! Very often one error causes another and when you fix the first error other errors go away, while some new ones may appear. Trying to fix errors lower down the list is a total waste of time!**

In this case we get the error: *Error C3861 'glPushMatrix': identifier not found*. This seems strange as we have just moved this code we haven't changed it. However what we have forgotten is the imports, so when compiling the Obstacle class the compilers doesn't know anything about OpenGL and glut. Therefore add the following to the top of Obstacle.h. 

Note that it is only needed in the header file, not in the .cpp file, as the .cpp file imports the header file. If you are using a mac or a linux machine you need slightly different includes.

```C++
/* OPENGL_INCLUDES */
#ifndef OPENGL_INCLUDES
#define OPENGL_INCLUDES
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif 
```

If you now run the project, it runs but the Obstacle has disappeared. If it has not, you not have removed the drawing code from StartGame.cpp.

What is needed next is for startGame.cpp to actually make use of the Obstacle class. In StartGame.cpp, at the top, after the includes but before the void drawScene add a variable of type Obstacle. When you do this the editor will complain that it does not know Obstacle. We have to import the header to make it work. It should look something like this:

```C++
....
#endif
#include "Obstacle.h";

Obstacle firstObstacle;

// Drawing routine.
void drawScene()
....
```

If we run this, it still doesn't draw the Obstacle. To make it draw we'll have to call drawScene. To do this locate the drawScene method in StartGame. It is important where we place this. It should be after the *gluLookAt* but before the *glutSwapBuffers*. It would also be good not to interfere with the Hovercraft. Therefore a good place would be just before the Hovercraft comment. Also note that it is now clear what this does and we don't really need to add a comment anymore!

```C++
....
	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	firstObstacle.drawScene();

	//Hovercraft
	glPushMatrix();
....
```
Now it works. We can add another Obstacle, let's call it second Obstacle and repeat the process above. However if we do that it will look the same, as they are drawn on top of each other. To fix this the Obstacle class will need to be able to keep track of it's position and draw itself there, so that we can create two Obstacle in two different positions.

To do this we are going to change Obstacle.h and Obstacle.cpp we'll use a glm::vec3 to indicate the position (in x,y,z coordinates). There is a function without a return type called *Obstacle()* this is called the constructor and here it defines what happens when you declare a variable of type Obstacle. The class should also store it's position so let's give it a private variable for that.

We will change it in Obstacle.h to:

```C++
....
# include <glm/glm.hpp> 
class Obstacle
{
private:
	glm::vec3 position;
public:
	Obstacle(glm::vec3 position);
...
```

And in Obstacle.cpp to the following, making sure we store the position in the class:

```C++
Obstacle::Obstacle(glm::vec3 position)
{
	this->position = position;
}
```

Now we will get an error from StartGame, complaining that the default constructor does not exist. If you had left the old one, there would be no complaint. This is because when we created an Obstacle ( *Obstacle firstObstacle;* ) in the startGame.cpp, we did not pass any variables in. In our case we prefer not to leave the default constructor as we don't want Obstacles without position. 

In StartGame.cpp we can add a second Obstacle and change the Obstacle declaration to:

```C++
Obstacle firstObstacle = Obstacle( glm::vec3(0,0,0) );
Obstacle secondObstacle = Obstacle( glm::vec3(2,2,2) );
``` 

And don't forget to add the drawScene call to the second Obstacle!

We still only see one as the position in the Obstacle is not used in deciding where to draw it. We could can add code to do this in the drawScene. In fact the drawScene is re-initialising the drawing object every time, while we only need to make sure it is drawn in the correct position, after we set it up once. It would also be nice if we could have a separate *start* and *update* method, like the Unity game engine for example. Go ahead and add void start() and void update() to the Obstacle class. For the update we will need to know the time since the last time the update was run. For example if the Obstacle was moving we would use this to calculate how much to move it.

*Obstacle.h*
```C++
void start();
void update(int deltaTime);
``` 

*Obstacle.cpp*
```C++
void Obstacle::start(){
}

void Obstacle::update(int deltaTime){
}
``` 

It would also be nice to separate drawing setup from the start to create a clear seperation between logic and drawing code, which after all is one of the main reasons for creating an engine in the first place. Therefore let's introduce another method called *setupDrawing* we will give this an integer called listBase as input. We'll see why shortly. 

To the setup method (which is called from the main) first add a call to setupDrawing and then to start for both Obstacles.

At this point we need to explain drawlists.

Display lists essentially are just integers that are used identify bits of the drawing code that should be treated more or less independently. In 212CR you will see more detailed explanation, but for us the main feature is that we can reliably move and change individual objects without affecting everything else.

We need a reliable way to create a unique number for each object, so that the two Obstacles will have a different display list. An easy way to do is is to introduce a variable called base in StartGame, indicating base (starting) display list, which we pass into setupDrawing. We let setupDrawing modify and return this variable, so we we make the call *base = firstObstacle.setupDrawing(displaylist)*.

We'll need to initialise this in the setup before we can do the setupDrawing of the Obstacle. We will need to indicate how many lists we will need. For now let's assume 1 list per object, so:

StartGame.cpp
```C++
...
// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	unsigned int base = glGenLists(2); // Generate display list base. 
	base = firstObstacle.setupDrawing(base);
	base = secondObstacle.setupDrawing(base);

	firstObstacle.start();
	secondObstacle.start();
}
...
``` 

**Please note** It would be possible to pass the parameters by reference and modify base, without the need for the assignment. However we prefer as much as possible to avoid our functions having unexpected side effects on variables outside their immediate scope (function or class they are in) as this makes it easier to understand what is happning.

We now will need to move the drawing code to setupDrawing and add the code to redraw our Obstacle in the right position to the drawScene:

*Obstacle.cpp*
```C++
...
unsigned int  Obstacle::setupDrawing(unsigned int listBase) {
	this->base = ++listBase;
	//glNewList(this->base, GL_COMPILE);
	glNewList(this->base, GL_COMPILE);
		glPushMatrix();
			glColor3f(0.55, 0.27, 0.07);
			glutSolidSphere(1.0, 40, 40);
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
...
``` 

You will need to add the new unsigned int base to Obstacle.h.

The resulting full code can be downloaded from this week's folder in github.

![Amber Traffic light](https://github.coventry.ac.uk/217CR-1718JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/amber.png?raw=true "You are encouraged to complete the following steps to enhance your learning")
## Obstacle Colour
Add a colour property to the Obstacle and make sure that it uses this to draw the correct colour.
You can define a colour as it's own class, or if you want to create a simple grouping of variables without inheritance possibilities you could use a struct:

```C++
struct Colour {
	float r, g, b;
};
``` 

## Game engine design (class structure)

Before we continue with the next tutorials it is worth considering the class structure for the game engine we are going to make. The rest of the tutorial is based based around this structure. It features a very simple GameEngine class which contains a list of GameObjects and it makes sure each GameObject gets draw messages, the message to run an update and keyboard inputs. This is by no means the perfect way to build a game engine. Have a look at the class diagram below.

![GameEngine class Diagram](https://github.coventry.ac.uk/217CR-1718JANMAY/TEACHING-MATERIALS/blob/master/Week2/tutorial-images/ClassDiagram.png?raw=true "Class diagram")

## Documentation

Commenting in code is very important, however beginning programmers often find it difficult at what level comments should be made. Comments are there to make things clearer and not just for the sake of it. So for example in the StartGame.cpp from week 1 we saw a comment followed by some drawing code as follows:

```C++
	//Obstacle
	glPushMatrix();
		glColor3f(0.55, 0.27, 0.07);
		glutSolidSphere(1.0, 40, 40);
	glPopMatrix();
``` 

This comment is very helpful as it is not easy to see from the code that this is supposed to represent an Obstacle. However after we have introduced the Obstacle class it gets reduced to one call and by choosing a good name for the class and the function, we don't really need a comment anymore to understand it:

```C++
	firstObstacle.drawScene();
``` 

Comments that explain what your classes do and what their functions and variables are for are essential to other other programmers and your future self (you'll be amazed how much you forget in just a few weeks!). Again the more obvious the naming and structre, the fewer comments needed.

Documentation is often required in bigger projects. Documentation explains the structure of the code and what each bit is for. It is not the same as commenting. Many programming languages have tools to help create documentation. Doxygen is a popular C++ documentation generation tool. "All" you have to do is create comments in the correct format, create a setup file and run Doxygen. This results in a set of web page documents that document your code and these are very useful for programmers using your project.

Work through the following tutorial and generate some doxygen documentation for this week's code.

https://www.stack.nl/~dimitri/doxygen/manual/starting.html

![Red Traffic light](https://github.coventry.ac.uk/217CR-1718JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/red.png?raw=true "The following are advanced steps you can complete when you are done with everything else.")
## Improving the game engine design
As this is by no means the perfect game engine, can you think how you would structure a basic game engine better? Are there things missing, things we will not be able to do or inefficiencies in the engine?
Draw a class diagram of a possible improved engine.
