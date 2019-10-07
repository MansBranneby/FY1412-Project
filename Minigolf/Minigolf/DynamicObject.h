#pragma once
#include "GameObject.h"

using namespace DirectX;

struct Environment //Jag gillar inte att Dobject har Environment. Men eftersom include game.h inte fungerar...
{
	float airDensity;
	XMVECTOR windVelocity;

	XMVECTOR gravAcc;
};

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
	XMVECTOR _velocity; 
	XMVECTOR _angularVelocity; //Spinn 
	
	float _mass, _fricGlide, _fricRoll;

	XMVECTOR calculateDrag(Environment* environment);
	XMVECTOR calcProjectile(float deltaSeconds, XMVECTOR acceleration);
	XMVECTOR calcGliding(float deltaSeconds, XMVECTOR acceleration);
	XMVECTOR calcRolling(float deltaSeconds, XMVECTOR acceleration);

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();
	
	void move(XMVECTOR acceleration, float deltaSeconds);

	//Physics
	XMVECTOR calculateMovement(float deltaSeconds, Environment* environment); //Calculate new positions of dynamic objects based on physics

	//GET & SET
	virtual void setVelocity(XMVECTOR velocity);
	virtual void setAngularVelocity(XMVECTOR angularVelocity);
	virtual void setMeansOfMovement(MeansOfMovement meansOfMovement);
	virtual void setMass(float mass);
	//virtual void setAcceleration(DirectX::XMVECTOR acceleration);
	virtual XMVECTOR getVelocity();
	virtual MeansOfMovement getMeansofMovement() const;
	virtual float getMass() const;
	//virtual DirectX::XMVECTOR getAcceleration();
};