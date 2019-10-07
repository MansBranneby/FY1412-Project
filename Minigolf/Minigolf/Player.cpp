#include "Player.h"

Player::Player()
{
	getGeometry()->setAngularVelocity(DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0));
	getGeometry()->setVelocity(DirectX::XMVectorSet(20.0, 20.0, 0.0, 1.0));

	getGeometry()->setMass(0.049f);
	getGeometry()->setMeansOfMovement(MeansOfMovement(REST));
}

Player::Player(DynamicObject * geometry)
{
	_geometry = geometry;
	getGeometry()->setAngularVelocity(DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0));
	getGeometry()->setVelocity(DirectX::XMVectorSet(20.0, 20.0, 0.0, 1.0));

	getGeometry()->setMass(0.049f);
	getGeometry()->setMeansOfMovement(MeansOfMovement(PROJECTILE));
}

Player::~Player()
{
}

void Player::handleInput(DirectX::Keyboard::State kb, DirectX::GamePad::State gp, float deltaSeconds)
{
	
}

DynamicObject * Player::getGeometry()
{
	return _geometry;
}


