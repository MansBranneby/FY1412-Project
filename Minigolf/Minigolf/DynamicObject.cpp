#include "DynamicObject.h"

XMVECTOR DynamicObject::calculateDrag(Environment * environment)
{
	XMVECTOR drag;

	switch (_meansOfMovement)
	{
	case PROJECTILE:
	{
		XMVECTOR relVel = _velocity - environment->windVelocity; //Velocity relative to wind
		float absRelVel = sqrt(pow(XMVectorGetX(relVel), 2) + pow(XMVectorGetY(relVel), 2) + pow(XMVectorGetZ(relVel), 2)); //The length of vector relVel
		
		float ballRadius = 0.0214f; //Later from geometry??
		float ballArea = XM_PI * pow(ballRadius, 2);

		float absVel = sqrt(pow(XMVectorGetX(_velocity), 2) + pow(XMVectorGetY(_velocity), 2) + pow(XMVectorGetZ(_velocity), 2)); //The length of vector velocity
		float dragCoeff = 0.53f - ((5.1f * absVel)/1000); //Tillfälligt sätt! Senare måste man nog anropa en funkiton i geometry eller liknande för att få detta. Då detta beror på geometrin men beroende av hastighet.

		drag = -(0.5f * environment->airDensity * ballArea * dragCoeff * absRelVel) * relVel; //The air drag on the object
		break;
	}
	case GLIDING:
		break;
	case ROLLING:
		break;
	default:
		break;
	}

	return drag;
}

XMVECTOR DynamicObject::calcProjectile(float deltaSeconds, XMVECTOR acceleration)
{
	XMVECTOR newPosition = getPositionVector() + (getVelocity() * deltaSeconds);
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

DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objType, BoundingType boundingType, XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, objType, BoundingType(boundingType), startingPosition, modelFile)
DynamicObject::DynamicObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, BoundingType(boundingType), startingPosition, modelFile)
{
	//_acceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
	_velocity = { 0.0f, 0.0f, 0.0f, 0.0f };
	//_friction = 0.000000001f;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::move(XMVECTOR acceleration, float deltaSeconds)
{
	//_velocity += acceleration;
	//_velocity *= pow(_friction, deltaSeconds);
	//setPosition(getPositionVector() + _velocity);
 //   
	//// Update transformation matrix
	//updateTransformations(getPositionFloat3());
	//getBoundingVolume()->move(acceleration);
}

XMVECTOR DynamicObject::calculateMovement(float deltaSeconds, Environment* environment)
{
	//Bättre att räkna ut analytiskt en gång för att sedan använda t och ursprunglig beräkning?
	
	//Forces
	XMVECTOR dragForce = calculateDrag(environment); //Drag
	XMVECTOR gForce = environment->gravAcc * 0.0459; //Gravity (*massa så länge environment har acc och inte force)
	//Magnus force
	XMVECTOR resForce = dragForce + gForce; //Resultant

	//Acceleration
	XMVECTOR acceleration = resForce / 0.0459; // a = F/m. Ska massan finnas i geometry eller i gameobject?

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
	default:
		break;
	}

	//Update objects transformation matrix and return
	updateTransformations(XMFLOAT3(XMVectorGetX(newPosition), XMVectorGetY(newPosition), XMVectorGetZ(newPosition)));
	return newPosition;
}

void DynamicObject::setVelocity(XMVECTOR velocity)
{
	_velocity = velocity;
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
