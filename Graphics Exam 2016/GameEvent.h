#pragma once

//An emun for all the possible actions in the game

enum class ActionEnum:int {
	NOACTION = 0,
	LEFT, // movment
	RIGHT,
	FORWARD,
	BACKWARD,
	MOUSEMOTION,
	NIGHTTOGGLE
};

/* This struct deals with an agent creating events.  This has an agent number and the action*/
struct GameEvent
{
	int agent;
	ActionEnum action;
};

