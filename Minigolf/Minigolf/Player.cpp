#include "Player.h"

Player::Player()
{
	this->setAngularVelocity(DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0));
	this->setVelocity(DirectX::XMVectorSet(20.0, 20.0, 0.0, 1.0));

	this->setMass(0.049f);
	this->setMeansOfMovement(MeansOfMovement(PROJECTILE));
}

Player::Player(DynamicObject * geometry)
{
	_geometry = geometry;
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


