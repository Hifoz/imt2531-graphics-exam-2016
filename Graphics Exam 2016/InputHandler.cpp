#include "InputHandler.h"

#include <iostream>
#include <queue>
#include <map>

/*
To add a new key change: 1) InputHandler eventRepeatRate 2) IntputHandler eventRepeat  3) handleKeys KeyDown 4) handleKeys KeyUp
To add a new Game Event add it to the GameEvent.h ActionEnum
*/

InputHandler::InputHandler()
{
	// add a repeat rate
	eventRepeatRate[ActionEnum::LEFT] = 0; // this is the cooldown in update cycles - currently 10 - repeatRate = 0 is every update
	eventRepeatRate[ActionEnum::RIGHT] = 0; // non repeating keys have eventRepeatRate[action] = INACTIVE for keyboard default or INT_MAX for almost never repeat.
	eventRepeatRate[ActionEnum::FORWARD] = 0;// = INACTIVE;  // This currently results in the standard key repeat of the Window keyboard
	eventRepeatRate[ActionEnum::BACKWARD] = 0;// You need to add to here to have new keys 
	eventRepeatRate[ActionEnum::MOUSEMOTION] = 0;// You need to add to here to have new keys 
	eventRepeatRate[ActionEnum::NIGHTTOGGLE] = 10;// You need to add to here to have new keys 
	

	// make sure it is initialised to inactive
	eventRepeat[ActionEnum::LEFT] = INACTIVE;
	eventRepeat[ActionEnum::RIGHT] = INACTIVE; 
	eventRepeat[ActionEnum::FORWARD] = INACTIVE;
	eventRepeat[ActionEnum::BACKWARD] = INACTIVE;	// Make sure all new events are inactive
	eventRepeat[ActionEnum::MOUSEMOTION] = INACTIVE;
	eventRepeat[ActionEnum::NIGHTTOGGLE] = INACTIVE;
	
	
}

/* Handles the keyboard events.  Updates the events queue with new Game Events. quits on esc*/
bool InputHandler::handleKeys(SDL_Event &eventHandler, std::queue<GameEvent>& events)
{
	ActionEnum action;
	switch (eventHandler.type){
	case SDL_KEYDOWN:
		switch (eventHandler.key.keysym.sym){
		case SDLK_a:
			action = ActionEnum::LEFT;
			break;
		case SDLK_s:
			action = ActionEnum::BACKWARD;
			break;
		case SDLK_d:
			action = ActionEnum::RIGHT;
			break;
		case SDLK_w:
			action = ActionEnum::FORWARD;
			break;
		case SDLK_n:
			action = ActionEnum::NIGHTTOGGLE;
			break;

		// 3) add the case for which key to set to the action


		case SDLK_ESCAPE:
			return false;
			break;
		}
		//This checks if the event is active and on cool down
		if (eventRepeat[action] < 0){
			events.push(GameEvent{ 0, action }); //trigger event
			eventRepeat[action] = eventRepeatRate[action]; // set the repeat cool down
		}

		break;
	case SDL_KEYUP:
		switch (eventHandler.key.keysym.sym){
		case SDLK_a:
			action = ActionEnum::LEFT;
			break;
		case SDLK_s:
			action = ActionEnum::BACKWARD;
			break;
		case SDLK_d:
			action = ActionEnum::RIGHT;
			break;
		case SDLK_w:
			action = ActionEnum::FORWARD;
			break;
		case SDLK_n:
			action = ActionEnum::NIGHTTOGGLE;
			break;

			// 4 ) make sure the keyup resets the event to inactive
			// remember to add the key up to turn off repeating.

		default:
			break;
		}
		eventRepeat[action] = INACTIVE;
		break;

	default:
		break;
	}
	return true; // no exit key pressed.
}

void InputHandler::handleMouse(SDL_Event &eventHandler, std::queue<GameEvent>& events)
{
	ActionEnum action = ActionEnum::NOACTION;

	switch (eventHandler.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		break;
	case SDL_MOUSEMOTION:
		mousePosition.x = eventHandler.motion.x;
		mousePosition.y = eventHandler.motion.y;
		mouseDelta.x = eventHandler.motion.xrel;
		mouseDelta.y = eventHandler.motion.yrel;
		action = ActionEnum::MOUSEMOTION;
		break;
	}

	if (eventRepeat[action] < 0) {
		events.push(GameEvent{ 0, action }); //trigger event
		eventRepeat[action] = eventRepeatRate[action]; // set the repeat cool down
	}
}


InputHandler::~InputHandler()
{
}

//returns continue true for continue
bool InputHandler::processEvents(SDL_Event& eventHandler, std::queue<GameEvent>& events)
{
	mouseDelta = glm::vec2(0);
	bool keyContinue = true;
	//Handle new events on queue
	while (SDL_PollEvent(&eventHandler) != 0)
	{
		//User requests quit
		if (eventHandler.type == SDL_QUIT){
			return false;
		}
		else {
			keyContinue = handleKeys(eventHandler, events);
			handleMouse(eventHandler, events);
		}
	}
	/*This will trigger all events that have reached cool down */
	for (auto it = eventRepeat.begin(); it != eventRepeat.end(); ++it){
		if (it->second != INACTIVE){
			if (it->second == 0){
				events.push(GameEvent{ 0, it->first }); // raise the event
				it->second = eventRepeatRate[it->first];  // set to cool down
			}
			else {
				--it->second;  // cool down toward triggering
			}
		}
	}
	return keyContinue;
}



