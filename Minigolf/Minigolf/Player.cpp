#include "Player.h"

void Player::hitBall(XMMATRIX rotationMat, float clubSpeed, ClubType clubType)
{
	Club club = _clubs[clubType];

	XMVECTOR ep, en;
	float up, un;
	float e = 0.78f; //TWO-PIECE

	ep = XMVector2Transform(XMVectorSet(cos(club.loft), sin(club.loft), 0.0f, 0.0f), rotationMat);
	en = XMVector2Transform(XMVectorSet(sin(club.loft), -cos(club.loft), 0.0f, 0.0f), rotationMat);

	up = ((1.0f + e) * club.mass * cos(club.loft) * clubSpeed) / (club.mass + _ball->getMass());
	un = (2.0f * club.mass * sin(club.loft) * clubSpeed) / (7 * (club.mass + _ball->getMass()));
	
	_ball->setVelocity(up * ep + un * en); //Set velocity of ball
	_ball->setAngularVelocity((un / 0.0214f) * XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); //Set backspinn //Ersätt med _ball->getRadius() när det fungerar
	_ball->setMeansOfMovement(MeansOfMovement(PROJECTILE)); //Hit the ball
}

Player::Player()
{
	/*getBall()->setAngularVelocity(DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0));
	getBall()->setVelocity(DirectX::XMVectorSet(20.0, 20.0, 0.0, 1.0));

	getBall()->setMass(0.049f);
	getBall()->setMeansOfMovement(MeansOfMovement(REST));*/
}

Player::Player(DynamicObject * ball)
{
	_ball = ball;
	_ball->setAngularVelocity(DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0));
	_ball->setVelocity(DirectX::XMVectorSet(20.0, 20.0, 0.0, 1.0));

	_ball->setMass(0.0459f);
	_ball->setMeansOfMovement(MeansOfMovement(REST));
	hitBall(XMMatrixRotationY(0.0f), 40.0f, IRON_7);
}

Player::~Player()
{
}

void Player::handleInput(DirectX::Keyboard::State kb, DirectX::GamePad::State gp, float deltaSeconds)
{
	
}

DynamicObject * Player::getBall()
{
	return _ball;
}


