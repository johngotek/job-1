# Session 5 Basic collision detection
![Green Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/green.png?raw=true "You should complete the following steps as a minimum in order to keep up with the tutorial")

In this session we will introduce very basic and naive collision detection. We will create the class structure a little more generic, with a Collider class that is extended by a CubeCollider. Each GameObject has a collider and within the GameEngine every time the updateGame is called, we will check for each game object whether it's collider collides with another object's collider. As we are only implementing cube colliders that is a fairly simple test at this stage, but the situation becomes more complex if we introduce more complex colliders.

Go ahead and add Collider.h. This will be a pure virtual class and thus we don't even need a Collider.cpp. In many other languages this may be called an interface. It's just to ensure all colliders have standardised function names etc. and it is quite likely we will have to adapt this as we introduce more complex colliders.

*Collider.h*
```C++

class Collider{
protected:
	glm::vec3* colliderCentre = NULL; ///<Centre point for the collider @note Usually the same as the gameobject it is a collider for.
public:
	virtual bool collidesWith(Collider* other) = 0; ///<pure virtual function. very collider must implement collidesWith, which calculates whethwer this collider collides with other.

	/* Every collider must implement the following pure virtual function or otherwise cause a compile error.*/
	virtual float minX() = 0; ///<Lowest x value for the collider. Must override in inherited classes.
	virtual float maxX() = 0; ///<Highest x value for the collider. Must override in inherited classes.
	virtual float minY() = 0; ///<Lowest y value for the collider. Must override in inherited classes.
	virtual float maxY() = 0; ///<Highest y value for the collider. Must override in inherited classes.
	virtual float minZ() = 0; ///<Lowest z value for the collider. Must override in inherited classes.
	virtual float maxZ() = 0; ///<Highest z value for the collider. Must override in inherited classes.
	virtual void Draw()=0; ///<Draw method for debug mode. Must override in inherited classes.
};
...
```

You will need to repeat similar declarations in CubeCollider.h. We can calculate minX,minY, maxX, maxY etc. based on the position and width, height and length for example minX:

*CubeCollider.cpp*
```C++
...
float CubeCollider::minX() {
	return this->colliderCentre->x - this->width / 2;
}
...
```

Next we implement collidesWith. When do 2 cubes collide? Let's simplify the problem by looking at it in 2D first. When do squares collide? Look at the following drawing.

![Diagram of overlapping rectangles](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week5/tutorial-images/rectanglesOverlap.png?raw=true "Overlapping rectangles")


As you can see the two squares overlap is and only if they overlap in both X and Y axes. Let's take the X axis. The two overlap if the left most point of the black one (minX) is between the left most point and the right most point of the green one, which is the situation shown in the diagram. They also overlap of the rightmost point of the black one is between the green ones.

This can be written with S1 and the green one and S2 as the black one as: 

```C++
(S2.minX >= S1.minX && S2.minX <= s1.maxX) || (S2.maxX >= S1.minX && S2.maxX <= s1.maxX)
...
```

We can simplify this, using boolean logic, or by just looking at the diagram we realise that the black one only overlaps if:

```C++
S2.minX() <= S1.maxX() && S2.maxX() >= S1.minX();
...
```

So our collidesWith becomes as follows. Note that we introduce a check to see if the collider is NULL. It is possible that a GameObject has no collider but the collideswith is still called. Of course we can't collide with a collider that doesn't exist!

*CubeCollider.cpp*
```C++
...
bool CubeCollider::collidesWith(Collider* other) {
	if (other == NULL) {
		return false;
	}

	bool overlapX = other->minX() <= this->maxX() && other->maxX() >= this->minX();
	bool overlapY = other->minY() <= this->maxY() && other->maxY() >= this->minY();
	bool overlapZ = other->minZ() <= this->maxZ() && other->maxZ() >= this->minZ();
	return overlapX && overlapY && overlapZ;
}
...
```

Next we need to adjust the GameObject to have a collider and the Hovercraft and Obstacle to initialise a box collider.


*Hovercraft.h*
```C++
class GameObject{
...
public:
...
	Collider* collider = NULL;  ///<Pointer to a collider for the game object. Initially NULL to indicate that we do not want to calculate collisions.
...
```

*Hovercraft.cpp*
```C++
...
Hovercraft::Hovercraft(glm::vec3 pos) :GameObject(pos) {
	Hovercraft::collider = new CubeCollider(&this->position,1.5,1.5,1.5);
}

Hovercraft::~Hovercraft(){
	delete Hovercraft::collider;
	Hovercraft::collider = NULL;
}
...
...
```

*Obstacle.cpp*
```C++
Obstacle::Obstacle(glm::vec3 pos, Colour col) : GameObject(pos) {
	this->colour = col;
	Obstacle::collider = new CubeCollider(&this->position,2, 2, 2);
}

Obstacle::~Obstacle(){
	delete Obstacle::collider;
	Obstacle::collider = NULL;
}
...
```

We will also introduce a collides function, first a virtual one (that can be overriden if you wish) in Gameobject and then specific ones in Hovercraft and Obstacle.



*GameObject.h*
```C++
class GameObject{
...
public:
...
	virtual void collides(Collider* other); ///<Virtual function to notify this game object that a collision has happened can be overridden (optional).
...
```

*Hovercraft.cpp*
```C++
...
void Hovercraft::collides(Collider* other) {
	if (debugMode) {
		std::cout << "Hovercraft collides!" << std::endl;
	}
}
...
```

*Obstacle.cpp*
```C++
void Obstacle::collides(Collider* other) {
	if (debugMode) {
		std::cout << "Obstacle collides, Obstacle will be removed!" << std::endl;
	}
	this->active = false;
}
...
```

Finally we need to update updateGame in the GameEngine to include the check. For this tutorial we will just add the the naive (O(N^2) check) we can easily improve slightly to 1/2 N^2 by not double checking, but it's possible to do even better with different tests.

*GameEngine.cpp*
```C++
...
	//Test collisions in a naive way for every pair of gameobjects (note calculations will explode when you increase the amount of gameobjects. Can you think of a better way?
	for (std::vector<GameObject*>::size_type i = 0; i != gameobjects.size(); i++) {
		for (std::vector<GameObject*>::size_type j =i+1 ; j != gameobjects.size(); j++) {
			if (gameobjects[i]->collider != NULL) {
				if (gameobjects[i]->collider->collidesWith(gameobjects[j]->collider)) {
					gameobjects[i]->collides(gameobjects[j]->collider);
					gameobjects[j]->collides(gameobjects[i]->collider);
				}
			}
		}
	}
...
```

![Amber Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/amber.png?raw=true "You are encouraged to complete the following steps to enhance your learning")
## More efficient collision detection
Investigate and implement improvements to the collision detection mechanism, such as limiting technqiues or different types of collision tests.

## More efficient collision detection
Improve the movement for the Hovercraft by basing it on Physics formulas (e.g. Newton's laws). To this end you may need to research whether and how this works with hovercraft.
