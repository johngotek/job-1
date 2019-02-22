# Session 1 Creating C++ project with some static graphics code

![Green Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/green.png?raw=true "You should complete the following steps as a minimum in order to keep up with the tutorial")
## Creating the project

In this session, we are going to create a new C++ project which will eventually become your game.

To this end go to file -&gt; new -&gt; project and choose _Empty Project_ under C++.

Give the project a sensible name, make sure that _create directory for solution_ is unticked and that you choose a suitable location to store the files. (See below)

![Screenshot showing creation of a new project](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week1/tutorial-images/001.png?raw=true "Creating a new project")

You now have an empty project. Make sure that at the top of the editor the project is set to Debug and X64:

![Screenshot of visual studio top bar](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week1/tutorial-images/002.png?raw=true "Make sure the project is set to x64 and debug")

Now right-click on source _Files_ -&gt; _Add_ -&gt; _New Item_ and choose _C++ File (.cpp)_. As a name choose something like _StartGame.cpp_.

You now have an empty file. This will be the main starting point for the program. In C++ this is indicated by having a function called main that return an int. In the empty file type the following code.

**Please be careful with copy and paste into Visual Studio as quotes and line endings are often not copied over correctly!**

```C++

int main(int argc, char **argv) {
        return 0;
}
```

Now press the button _Local Windows Debugger._ You will see the project compiling and being run. Of course, it doesn&#39;t really do anything yet.

Next we will need to include a number of libraries. There are 2 main ways to do this. **Please note these instructions are designed for windows and the lab machines, instructions for mac may vary slightly**

1. Include all files needed in the project folder
2. Refer to the necessary files placed elsewhere

We will use the first option. This has the advantage that the project is easier to move to a different machine. However, it increases the size of your project. Therefore it is important you only include the files that are necessary!

Within visual studio right-click on the name of your application (right under the solution) and select properties. Then choose the _C++_ tab. Under Additional include directories write _include_.

![Screenshot of project settings](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week1/tutorial-images/003.png?raw=true "Project settings for indicatingd ependancies")

Now go to the linker tab and write _include_ under _Additional Library Directories_.

![Screenshot of project settings](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/Week1/tutorial-images/004.png?raw=true "Project settings for indicatingd ependancies")

Next navigate to where you have stored the project. In my case this is _C:\Users\maurice\Documents\Visual Studio 2017\Projects\MyGame_ but **this will be different for you** in this folder create a new folder called _include_.

You now need to copy a number of files and folders. These can be found in the dependancies sub-folder of the teaching materials github: [https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/tree/master/Week1/dependancies](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/tree/master/Week1/dependancies)

Below is a table of what goes where. &lt;project&gt; stands for the place of your project folder.

| What to copy               | Where to copy to                                          |
| -------------------------- | ---------------------------------------------------------:|
| freeglut.dll               | &lt;project&gt;                                           |
| glew32.dlls                | &lt;project&gt;                                           |
| include (the whole folder) | &lt;project&gt; (so that you have &lt;project&gt;\include)|


The next step for todays session is to add some code to create a simple scene to the StartGame.cpp file. **This code can be found in the folder (Week 1) of the github repository (in StartGame.cpp)**. Import this code and run the project.

*Please note if you are using a mac or linux machine you will need slightly different includes*

![Amber Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/amber.png?raw=true "You are encouraged to complete the following steps to enhance your learning")
## Investigate what does what
Next play round with the elements in drawscene, commenting out the different parts. Can you tell which part is responsible for drawing what? Do the same in the main. Can you tell what each glut function is for? If you are unsure, look them up on the web or ask for help.

As you may have noticed the glut functions use, what is known as callbacks. The glutMainLoop() starts the game loop. idle() gets called from the main game loop when it has no work to do, and drawScene() gets called when it becomes necessary to redraw the scene. 
Can you tell why idle() makes a call to glutPostRedisplay()?

![Red Traffic light](https://github.coventry.ac.uk/217CR-1819JANMAY/TEACHING-MATERIALS/blob/master/tutorial-images/red.png?raw=true "The following are advanced steps you can complete when you are done with everything else.")
## Lambda functions in C++
The callbacks currently use functions and in the glut calls you place the name of the function (for example glutIdleFunc(idle) ).

You are only using these functions in the glut calls and some of them are very small indeed. Defining a separate function for this seems like a lot of effort. Also it can confuse you as a programmer. When reviewing the code you may think there are functions that are not needed. Therfore it would be nice if in the glut calls you could just tell it what work you would like it to do, rather than having to specify a function.
Fortunately you can actually do this using lambda functions (since C++ 11). Now have a look at the following guide and re-write some of the glut function calls. For example re-write glutKeyboardFunc(keyInput) to not use the keyInput function, but still close when ESC is pressed.

http://en.cppreference.com/w/cpp/language/lambda
