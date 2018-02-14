#pragma once

#include "GameObject.h"
#include "Mirror.h"

class Car : public GameObject
{
public:
	Car(std::string path);
	~Car();

	void moveCar(int dir, float dt);
	void turnCar(int dir, float dt);

	void setMirror(Mirror* mirror);
	Mirror* getMirror() { return m_mirror; }

private:
	int m_speed, m_turnspeed;

	Mirror* m_mirror;
};

