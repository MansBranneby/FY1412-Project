#include "Player.h"

Player::Player()
{
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


