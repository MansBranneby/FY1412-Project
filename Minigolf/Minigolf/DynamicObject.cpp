#include "DynamicObject.h"

DirectX::XMVECTOR DynamicObject::calcProjectile(Game * game, float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DirectX::XMVECTOR DynamicObject::calcGliding(Game * game, float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DirectX::XMVECTOR DynamicObject::calcRolling(Game * game, float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, Geometry geometryType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, Geometry(geometryType),startingPosition, modelFile)
DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, BoundingType boundingType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, BoundingType(boundingType), startingPosition, modelFile)
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

DirectX::XMVECTOR DynamicObject::calculateMovement(Game * game)
{
	DirectX::XMVECTOR newPosition;
	float deltaSeconds = game->getClock()->getDeltaSeconds(); //delta seconds

	switch (_meansOfMovement) // Means of movement
	{
	case PROJECTILE:
		newPosition = calcProjectile(game, deltaSeconds);
		break;
	case GLIDING:
		newPosition = calcGliding(game, deltaSeconds);
		break;
	case ROLLING:
		newPosition = calcRolling(game, deltaSeconds);
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
