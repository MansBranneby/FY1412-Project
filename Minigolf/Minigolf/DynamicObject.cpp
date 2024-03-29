#include "DynamicObject.h"

XMVECTOR DynamicObject::calcProjectile(float deltaSeconds, Environment *environment)
{
	XMVECTOR acceleration;

	//Forces
	XMVECTOR dragForce = calculateDrag(environment); //Drag //Change depending on vel!!!
	XMVECTOR gForce = environment->gravAcc * _mass; //Gravity
	
	//Magnus force
	float v = XMVectorGetX(XMVector3Length(_velocity));
	float w = XMVectorGetX(XMVector3Length(_angularVelocity));
	float Cm = v == 0.0f ? 0 : (sqrt(1.0f + 0.31f * (w / v)) - 1.0f) / 20.0f;
	float Fm = 0.5f * environment->airDensity * pow(0.0214f, 2.0f) * 2.0f * XM_PI * Cm * pow(v, 2.0f);
	XMVECTOR mForce = XMVector3Normalize(XMVector3Cross(_velocity, _angularVelocity));
	mForce = mForce * Fm; //Magnus force
	
	XMVECTOR resForce = dragForce + gForce + mForce; //Resultant

	//Acceleration
	acceleration = resForce / _mass; // a = F/m.

	_rotation += _angularVelocity * deltaSeconds;
	XMVECTOR newPosition = getPositionVector() +(getVelocity() * deltaSeconds);
	setVelocity(getVelocity() + acceleration * deltaSeconds);
	return newPosition;
}

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, DYNAMICOBJECT, BoundingType(boundingType), startingPosition, modelFile)
{
	setCup(false);
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	_surfaceNormal = { 0.0f, 0.0f, 0.0f, 0.0f };
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(XMVECTOR acceleration, float deltaSeconds)
{
}

XMVECTOR DynamicObject::calculateMovement(float deltaSeconds, Environment* environment)
{
	XMVECTOR newPosition; //Return value

	switch (_meansOfMovement) // Means of movement
	{
	case PROJECTILE:
		newPosition = calcProjectile(deltaSeconds, environment);
		break;
	case GLIDING:
		newPosition = calcGliding(deltaSeconds, environment);
		break;
	case ROLLING:
		newPosition = calcRolling(deltaSeconds, environment);
		break;
	case REST:
		newPosition = getPositionVector(); //No update
		break;
	default:
		break;
	}

	//Update objects transformation matrix and return
	DirectX::XMMATRIX rotMat;
	float rotGain = DirectX::XMVectorGetX(DirectX::XMVector3Length(_rotation));
	if (DirectX::XMVectorGetX(_angularVelocity) == 0.0f && DirectX::XMVectorGetY(_angularVelocity) == 0.0f && DirectX::XMVectorGetZ(_angularVelocity) == 0.0f)
		rotMat = DirectX::XMMatrixIdentity();
	else
		rotMat = DirectX::XMMatrixRotationAxis(_angularVelocity, -rotGain);

	DirectX::XMMATRIX tranMat = DirectX::XMMatrixTranslation(getPositionFloat3().x, getPositionFloat3().y, getPositionFloat3().z);
	DirectX::XMMATRIX world = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranspose(tranMat), DirectX::XMMatrixTranspose(rotMat));
	// Update world matrices for models
	updateTransformations(world);

	return newPosition;
}

void DynamicObject::setVelocity(XMVECTOR velocity)
{
	_velocity = velocity; //Tvinga 4:e element till 1.0f??
}

void DynamicObject::setRotation(XMVECTOR rotation)
{
	_rotation = rotation;
}

void DynamicObject::setAngularVelocity(XMVECTOR angularVelocity)
{
	_angularVelocity = angularVelocity;
}

void DynamicObject::setSurfaceNormal(XMVECTOR surfaceNormal)
{
	_surfaceNormal = surfaceNormal;
}

void DynamicObject::setMeansOfMovement(MeansOfMovement meansOfMovement)
{
	_meansOfMovement = meansOfMovement;
}

void DynamicObject::setMass(float mass)
{
	_mass = mass;
}

XMVECTOR DynamicObject::getVelocity()
{
	return _velocity;
}

XMVECTOR DynamicObject::getRotation() const
{
	return _rotation;
}

XMVECTOR DynamicObject::getAngularVelocity()
{
	return _angularVelocity;
}

XMVECTOR DynamicObject::getSurfaceNormal() const
{
	return _surfaceNormal;
}

MeansOfMovement DynamicObject::getMeansofMovement() const
{
	return _meansOfMovement;
}

float DynamicObject::getMass() const
{
	return _mass;
}