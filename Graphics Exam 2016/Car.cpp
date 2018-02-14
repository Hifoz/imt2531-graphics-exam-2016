#include "Car.h"



Car::Car(std::string meshPath) : GameObject(meshPath)
{
	m_speed = 30;
	m_turnspeed = 100;

}


Car::~Car()
{
}


void Car::moveCar(int dir, float dt)
{
	transform.pos.z += dir * dt * m_speed * glm::cos(glm::radians(transform.rot.y));
	transform.pos.x += dir * dt * m_speed * glm::sin(glm::radians(transform.rot.y));
}


void Car::turnCar(int dir, float dt)
{
	transform.rot.y += dir * dt * m_turnspeed;
}


void Car::setMirror(Mirror* mirror) 
{
	m_mirror = mirror;
	m_mirror->setCarTransform(&transform);
}