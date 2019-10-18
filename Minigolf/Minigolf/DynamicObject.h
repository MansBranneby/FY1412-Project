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
	ROLLING,
	REST
};

class DynamicObject : public GameObject
{
private:
	//Variables for physics calculations
	MeansOfMovement _meansOfMovement;
	XMVECTOR _velocity; 
	XMVECTOR _angularVelocity; //Spinn 
	XMVECTOR _surfaceNormal;

	float _mass, _fricGlide, _fricRoll;

	virtual XMVECTOR calculateDrag(Environment* environment) = 0;
	XMVECTOR calcProjectile(float deltaSeconds, Environment *environment);
	XMVECTOR calcGliding(float deltaSeconds, Environment *environment);
	XMVECTOR calcRolling(float deltaSeconds, Environment *environment);

public:
	DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~DynamicObject();
	
	void move(XMVECTOR acceleration, float deltaSeconds);

	//Physics
	XMVECTOR calculateMovement(float deltaSeconds, Environment* environment); //Calculate new positions of dynamic objects based on physics
	virtual void calculateAfterColVel(GameObject* colObj) = 0;

	//GET & SET
	virtual void setVelocity(XMVECTOR velocity);
	virtual void setAngularVelocity(XMVECTOR angularVelocity);
	virtual void setSurfaceNormal(XMVECTOR surfaceNormal);
	virtual void setMeansOfMovement(MeansOfMovement meansOfMovement);
	virtual void setMass(float mass);
	//virtual void setAcceleration(DirectX::XMVECTOR acceleration);
	virtual XMVECTOR getVelocity();
	virtual XMVECTOR getAngularVelocity();
	virtual XMVECTOR getSurfaceNormal() const;
	virtual MeansOfMovement getMeansofMovement() const;
	virtual float getMass() const;
	//virtual DirectX::XMVECTOR getAcceleration();
};