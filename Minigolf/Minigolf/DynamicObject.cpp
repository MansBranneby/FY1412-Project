#include "DynamicObject.h"

DirectX::XMVECTOR DynamicObject::calcProjectile(float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DirectX::XMVECTOR DynamicObject::calcGliding(float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DirectX::XMVECTOR DynamicObject::calcRolling(float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, BoundingType(boundingType), startingPosition, modelFile)
{
	_acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	_friction = 0.000000001f;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(DirectX::XMVECTOR acceleration, float deltaSeconds)
{
	_velocity += acceleration;
	_velocity *= pow(_friction, deltaSeconds);
	setPosition(getPositionVector() + _velocity);
    
	// Update transformation matrix
	updateTransformations(getPositionFloat3());
	getBoundingVolume()->move(acceleration);
}

DirectX::XMVECTOR DynamicObject::calculateMovement(float deltaSeconds)
{
	DirectX::XMVECTOR newPosition;

	switch (_meansOfMovement) // Means of movement
	{
	case PROJECTILE:
		newPosition = calcProjectile(deltaSeconds);
		break;
	case GLIDING:
		newPosition = calcGliding(deltaSeconds);
		break;
	case ROLLING:
		newPosition = calcRolling(deltaSeconds);
		break;
	default:
		break;
	}

	return DirectX::XMVECTOR();
}

void DynamicObject::setVelocity(DirectX::XMVECTOR velocity)
{
	_velocity = velocity;
}

void DynamicObject::setAcceleration(DirectX::XMVECTOR acceleration)
{
	_acceleration = acceleration;
}

DirectX::XMVECTOR DynamicObject::getVelocity()
{
	return _velocity;
}

DirectX::XMVECTOR DynamicObject::getAcceleration()
{
	return _acceleration;
}
