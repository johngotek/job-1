/////////////////////////////////////////////////////////////////////////////////////////////        
/// StartGame.cpp
///
/// @brief This is the "main" which initialises the engine and starts the game.
///
/// @author Maurice Hendrix
///////////////////////////////////////////////////////////////////////////////////////////// 

#include "GameEngine/GameEngine.h"
#include "GameEngine/Grass.h"
#include "GameEngine/Hovercraft.h"
#include "GameEngine/Obstacle.h"

int main(int argc, char **argv) {

	//Initialise the engine.
	GameEngine::initEngine(argc, argv, "Hovercraft tutorial",true);

	//Adding grass field.
	GameEngine::addGameObject(new Grass(glm::vec3(0, 0, 0), glm::vec3(5, 0, 5)) );

	//Adding an Obstacle.
	GameEngine::addGameObject(new Obstacle(glm::vec3(0.0, 0.0, -2.0), { 0.55f, 0.27f, 0.07f }));
	
	//Adding an Obstacle with different colour.
	GameEngine::addGameObject(new Obstacle(glm::vec3(4.0, 0.0, 4.0)) );

	//Add a movable hovercraft.
	GameEngine::addGameObject(new Hovercraft(glm::vec3(-2, 0, 2)), false);


	//Start the game engine.
	GameEngine::startEngine();

	return 0;
}