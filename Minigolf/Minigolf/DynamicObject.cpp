#include "DynamicObject.h"

XMVECTOR DynamicObject::calcProjectile(float deltaSeconds, XMVECTOR acceleration)
{
	XMVECTOR newPosition = getPositionVector() +(getVelocity() * deltaSeconds);
	//setVelocity(getVelocity() + XMVectorSet(0.0f, -9.82 * deltaSeconds, 0.0f, 1.0f));
	setVelocity(getVelocity() + acceleration * deltaSeconds);
	return newPosition;
}

XMVECTOR DynamicObject::calcGliding(float deltaSeconds, XMVECTOR acceleration)
{
	return XMVECTOR();
}

XMVECTOR DynamicObject::calcRolling(float deltaSeconds, XMVECTOR acceleration)
{
	return XMVECTOR();
}

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, BoundingType(boundingType), startingPosition, modelFile)
{
	this->setObjectType(DYNAMICOBJECT);
	//_acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	//_friction = 0.000000001f;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(XMVECTOR acceleration, float deltaSeconds)
{
}

XMVECTOR DynamicObject::calculateMovement(float deltaSeconds, Environment* environment)
{
	XMVECTOR acceleration;
	if (_meansOfMovement != REST)
	{
		//Forces
		XMVECTOR dragForce = calculateDrag(environment); //Drag
		XMVECTOR gForce = environment->gravAcc * _mass; //Gravity
		//Magnus force
		XMVECTOR resForce = dragForce + gForce; //Resultant

		//Acceleration
		acceleration = resForce / _mass; // a = F/m.
	}

	XMVECTOR newPosition; //Return value

	switch (_meansOfMovement) // Means of movement
	{
	case PROJECTILE:
		newPosition = calcProjectile(deltaSeconds, acceleration);
		break;
	case GLIDING:
		newPosition = calcGliding(deltaSeconds, acceleration);
		break;
	case ROLLING:
		newPosition = calcRolling(deltaSeconds, acceleration);
		break;
	case REST:
		newPosition = getPositionVector(); //No update
		break;
	default:
		break;
	}

	//Update objects transformation matrix and return
	updateTransformations(XMFLOAT3(XMVectorGetX(newPosition), XMVectorGetY(newPosition), XMVectorGetZ(newPosition)));
	return newPosition;
}

void DynamicObject::setVelocity(XMVECTOR velocity)
{
	_velocity = velocity; //Tvinga 4:e element till 1.0f??
}

void DynamicObject::setAngularVelocity(XMVECTOR angularVelocity)
{
	_angularVelocity = angularVelocity;
}

void DynamicObject::setMeansOfMovement(MeansOfMovement meansOfMovement)
{
	_meansOfMovement = meansOfMovement;
}

void DynamicObject::setMass(float mass)
{
	_mass = mass;
}

//void DynamicObject::setAcceleration(DirectX::XMVECTOR acceleration)
//{
//	_acceleration = acceleration;
//}

XMVECTOR DynamicObject::getVelocity()
{
	return _velocity;
}

MeansOfMovement DynamicObject::getMeansofMovement() const
{
	return _meansOfMovement;
}

float DynamicObject::getMass() const
{
	return _mass;
}

//DirectX::XMVECTOR DynamicObject::getAcceleration()
//{
//	return _acceleration;
//}
