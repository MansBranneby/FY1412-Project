#pragma once
#include <DirectXMath.h>

class Club
{
private:
	float _mass;
	DirectX::XMVECTOR _acceleration;
public:
	Club();
	Club(float mass);
	
	void setMass(float mass);
	void setAcceleration(DirectX::XMVECTOR acceleration);

	float getMass();
	DirectX::XMVECTOR getAcceleration();
};