#include "Club.h"

Club::Club()
{
}

Club::Club(float mass)
{
	_mass = mass;
}

void Club::setMass(float mass)
{
	_mass = mass;
}

void Club::setAcceleration(DirectX::XMVECTOR acceleration)
{
	_acceleration = acceleration;
}

float Club::getMass()
{
	return _mass;
}

DirectX::XMVECTOR Club::getAcceleration()
{
	return _acceleration;
}
