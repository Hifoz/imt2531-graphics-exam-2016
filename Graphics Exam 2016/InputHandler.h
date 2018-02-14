#pragma once

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#include <SDKDDKVer.h>

	#include <SDL.h>
#include <glm\glm.hpp>
#endif

#include "GameEvent.h"
#include <map>
#include <queue>

#define INACTIVE -1

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	//Key repeat system if key still down issue event
	std::map<ActionEnum, int> eventRepeat; // this holds the count down to the next trigger of the event 
	std::map<ActionEnum, int> eventRepeatRate; // This holds the repeat rate per event

	bool processEvents(SDL_Event& eventHandler, std::queue<GameEvent>& events);
	bool handleKeys(SDL_Event &eventHandler, std::queue<GameEvent>& events);
	void handleMouse(SDL_Event &eventHandler, std::queue<GameEvent>& events);
	
	glm::vec2 getMouseDelta() { return mouseDelta; }

private:
	glm::vec2 mousePosition;
	glm::vec2 mouseDelta;
};

