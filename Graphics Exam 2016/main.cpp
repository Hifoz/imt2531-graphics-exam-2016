/* Template Author:Simon McCallum
Notes:
Created for the IMT2531 Graphics Exam 2015

To get the naming right I have set.
Project-> Properties -> Configuration properties -> General -> Target name  = exam2016.exe
Project-> Properties -> Configuration properties -> Debugging -> Commande Arguments = level01.dat

I suggest you read and understand how the input handler works.  It uses and event queue system.  
The keyboard events create game events which are pushed onto the GameEvent queue.  These are the
events which the program should actually repond to rather than the actual keyboard presses.

You should have environment variables set up for home of SDL GLEW and GLM so the calls below work
$(SDL_HOME)\include;$(GLEW_HOME)\include;$(GLM_HOME);$(IncludePath)


*/

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
	#include <SDL2/SDL_opengl.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#include <SDKDDKVer.h>

	#include <SDL.h>
	#include <gl\glew.h>
	#include <SDL_opengl.h>
	#include <gl\glu.h>
#endif

#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ 11 stuff to use
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>

#include "GameLevel.h"
#include "WindowHandler.h"
#include "InputHandler.h"
#include "ShaderHandler.h"
#include "LightHandler.h"
#include "GameEvent.h"
#include "Camera.h"

SDL_Event input;



// Process all events in the eventQueue
void update(std::queue<GameEvent>& eventQueue, InputHandler* inputHandler, Camera* camera, GameLevel* level, float dt)
{
	glm::vec2 mouseDelta;
	while (!eventQueue.empty())
	{
		GameEvent nextEvent = eventQueue.front();
		eventQueue.pop();

		if (nextEvent.action == ActionEnum::FORWARD) // use the Action Enum to decide what to do.
			level->getCar()->moveCar(1, dt);
		else if (nextEvent.action == ActionEnum::BACKWARD)
			level->getCar()->moveCar(-1, dt);
		else if (nextEvent.action == ActionEnum::LEFT)
			level->getCar()->turnCar(1, dt);
		else if (nextEvent.action == ActionEnum::RIGHT)
			level->getCar()->turnCar(-1, dt);
		else if (nextEvent.action == ActionEnum::MOUSEMOTION)
			camera->updateAngle(inputHandler->getMouseDelta() * dt);
		else if (nextEvent.action == ActionEnum::NIGHTTOGGLE)
			LightHandler::getInstance().toggleNighttime();

	}

	LightHandler::getInstance().updateCarLights(&level->getCar()->transform);
	camera->updatePosition();
	//SDL_Delay(30); // you need to control frame rate so the input events do not go crazy fast.
}

void draw(GameLevel* level, ShaderHandler* shaderHandler, Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	level->draw(shaderHandler, camera);
	SDL_GL_SwapWindow(WindowHandler::getInstance().getWindow());
}

/*
You need to give a better description
*/
#undef main
int main(int argc, char* argv[])
{
	float deltaTime = 0.0f; //Time since last pass through of the game loop.
	auto clockStart = std::chrono::high_resolution_clock::now(); //Clock used for timing purposes
	auto clockStop = clockStart;


	bool running = WindowHandler::getInstance().init();
	LightHandler::getInstance().init();
	SDL_Event eventHandler;
	GameLevel* currentLevel;
	std::queue<GameEvent> eventQueue;
	InputHandler inputs;
	TextureHandler* textureHandler = new TextureHandler();
	ShaderHandler* shaderHandler = new ShaderHandler();
	shaderHandler->initializeShaders();

	if (argc > 1) currentLevel = new GameLevel(textureHandler, std::string(argv[1]));
	else currentLevel = new GameLevel(textureHandler);

	Camera* camera = new Camera(currentLevel->getCar());
	

	if (!running) 
	{
		printf("Failed to initialize!\n");
		int a;
		std::cin >> a;
		return 1;
	}

	while (running){
		clockStart = std::chrono::high_resolution_clock::now();

		running = inputs.processEvents(eventHandler, eventQueue); // returns a quit and updates the eventQueue with events
		update(eventQueue, &inputs, camera, currentLevel, deltaTime);
		draw(currentLevel, shaderHandler, camera);

		clockStop = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(clockStop - clockStart).count();
	}
	
	return 0;
}