#include "DynamicObject.h"

DirectX::XMVECTOR DynamicObject::calcProjectile(float deltaSeconds)
{
	DirectX::XMVECTOR newPosition = getPositionVector() + (getVelocity() * deltaSeconds);
	setVelocity(getVelocity() + DirectX::XMVectorSet(0.0f, -9.82 * deltaSeconds, 0.0f, 1.0f)); //Måste ha tillgång till Environment från Game
	return newPosition;
}

DirectX::XMVECTOR DynamicObject::calcGliding(float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DirectX::XMVECTOR DynamicObject::calcRolling(float deltaSeconds)
{
	return DirectX::XMVECTOR();
}

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, BoundingType boundingType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, BoundingType(boundingType), startingPosition, modelFile)
{
	//_acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	//_friction = 0.000000001f;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(DirectX::XMVECTOR acceleration, float deltaSeconds)
{
	//_velocity += acceleration;
	//_velocity *= pow(_friction, deltaSeconds);
	//setPosition(getPositionVector() + _velocity);
 //   
	//// Update transformation matrix
	//updateTransformations(getPositionFloat3());
	//getBoundingVolume()->move(acceleration);
}

DirectX::XMVECTOR DynamicObject::calculateMovement(float deltaSeconds)
{
	//Bättre att räkna ut analytiskt en gång för att sedan använda t och ursprunglig beräkning?

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

	//Update objects transformation matrix and return
	updateTransformations(DirectX::XMFLOAT3(DirectX::XMVectorGetX(newPosition), DirectX::XMVectorGetY(newPosition), DirectX::XMVectorGetZ(newPosition)));
	return newPosition;
}

void DynamicObject::setVelocity(DirectX::XMVECTOR velocity)
{
	_velocity = velocity;
}

void DynamicObject::setAngularVelocity(DirectX::XMVECTOR angularVelocity)
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

DirectX::XMVECTOR DynamicObject::getVelocity()
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
