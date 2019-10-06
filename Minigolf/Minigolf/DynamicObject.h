#pragma once
#include "GameObject.h"

enum MeansOfMovement
{
	PROJECTILE,
	GLIDING,
	ROLLING
};

class DynamicObject : public GameObject
{
private:
	MeansOfMovement _meansOfMovement;
	DirectX::XMVECTOR _velocity; 
	DirectX::XMVECTOR _angularVelocity; //Spinn 
	DirectX::XMVECTOR _acceleration; //Ska vi nog inte ha i fysikspelet
	float _friction; //Ska vi nog inte ha i denna form i fysikspelet?

	DirectX::XMVECTOR calcProjectile(float deltaSeconds);
	DirectX::XMVECTOR calcGliding(float deltaSeconds);
	DirectX::XMVECTOR calcRolling(float deltaSeconds);

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();

	virtual void setVelocity(DirectX::XMVECTOR velocity);
	virtual void setAcceleration(DirectX::XMVECTOR acceleration);
	virtual DirectX::XMVECTOR getVelocity();
	virtual DirectX::XMVECTOR getAcceleration();
	
	void move(DirectX::XMVECTOR acceleration, float deltaSeconds);

	//Physics
	virtual DirectX::XMVECTOR calculateMovement(float deltaSeconds); //Calculate new positions of dynamic objects based on physics
};