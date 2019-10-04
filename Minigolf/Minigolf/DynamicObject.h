#pragma once
#include "GameObject.h"
//#include "Game.h"

enum MeansOfMovement
{
	PROJECTILE,
	GLIDING,
	ROLLING
};

class DynamicObject : public GameObject
{
private:
	//Variables for physics calculations
	MeansOfMovement _meansOfMovement;
	DirectX::XMVECTOR _velocity; 
	DirectX::XMVECTOR _angularVelocity; //Spinn 
	
	float _mass, _fricGlide, _fricRoll;

	DirectX::XMVECTOR calcProjectile(float deltaSeconds);
	DirectX::XMVECTOR calcGliding(float deltaSeconds);
	DirectX::XMVECTOR calcRolling(float deltaSeconds);

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();
	
	void move(DirectX::XMVECTOR acceleration, float deltaSeconds);

	//Physics
	DirectX::XMVECTOR calculateMovement(float deltaSeconds); //Calculate new positions of dynamic objects based on physics

	//GET & SET
	virtual void setVelocity(DirectX::XMVECTOR velocity);
	virtual void setAngularVelocity(DirectX::XMVECTOR angularVelocity);
	virtual void setMeansOfMovement(MeansOfMovement meansOfMovement);
	virtual void setMass(float mass);
	//virtual void setAcceleration(DirectX::XMVECTOR acceleration);
	virtual DirectX::XMVECTOR getVelocity();
	virtual MeansOfMovement getMeansofMovement() const;
	virtual float getMass() const;
	//virtual DirectX::XMVECTOR getAcceleration();
};