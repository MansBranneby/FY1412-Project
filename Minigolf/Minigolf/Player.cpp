#include "Player.h"

void Player::hitBall(XMMATRIX rotationMat, float clubSpeed, ClubType clubType)
{
	Club club = _clubs[clubType];

	XMVECTOR ep, en;
	float up, un;
	float e = 0.78f; //TWO-PIECE

	ep = XMVector3Transform(XMVectorSet(cos(club.loft), sin(club.loft), 0.0f, 0.0f), rotationMat);
	en = XMVector3Transform(XMVectorSet(sin(club.loft), -cos(club.loft), 0.0f, 0.0f), rotationMat);

	up = ((1.0f + e) * club.mass * cos(club.loft) * clubSpeed) / (club.mass + _ball->getMass());
	un = (2.0f * club.mass * sin(club.loft) * clubSpeed) / (7 * (club.mass + _ball->getMass()));
	
	_ball->setVelocity(up * ep + un * en); //Set velocity of ball
	_ball->setAngularVelocity((un / 0.0214f) * XMVector3Transform(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), rotationMat)); //Set backspinn //Ersätt med _ball->getRadius() när det fungerar
	_ball->setMeansOfMovement(MeansOfMovement(PROJECTILE)); //Hit the ball

	_nrOfHits++;
}

Player::Player()
{
}

Player::Player(DynamicObject * ball)
{
	_ball = ball;
	_ball->setMass(0.0459f);
	_yRotation = 0.0f;
	_clubSpeedIncrement = 20.0f;
	_ball->setMeansOfMovement(MeansOfMovement(REST));
	_nrOfHits = 0;
	_prevNrOfHits = 0;
	//hitBall(XMMatrixRotationY(0.0f), 40.0f, IRON_7);
}

Player::~Player()
{
}

void Player::setClubChoice(ClubType clubChoice)
{
	_clubChoice = clubChoice;
}

void Player::setPrevNrOfHits(int prevNrOfHits)
{
	_prevNrOfHits = prevNrOfHits;
}

void Player::setNrOfHits(int nrOfHits)
{
	_nrOfHits = nrOfHits;
}

void Player::handleInput(DirectX::Keyboard::State kb, DirectX::GamePad::State gp, DirectX::Mouse::State ms, float deltaSeconds)
{
	Keyboard::KeyboardStateTracker tracker;
	tracker.Update(kb);

	if (ms.positionMode == Mouse::MODE_RELATIVE)
		_yRotation += XMConvertToRadians((float)ms.x * deltaSeconds * 500);

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.A) //Left
		_yRotation -= 2.0f * deltaSeconds;
	if (kb.D) //Right
		_yRotation += 2.0f * deltaSeconds;

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(_yRotation - (XM_PI * 0.5f));

	if(ms.leftButton)
	{
		if (_clubSpeed < 0.0f)
			_clubSpeedIncrement *= -1.0f;
		if (_clubSpeed > 57.0f)
			_clubSpeedIncrement *= -1.0f;

		_clubSpeed += _clubSpeedIncrement * deltaSeconds;
	}
	if (kb.Space)
		hitBall(rotationMatrix, _clubSpeed, _clubChoice);

	if (kb.Space && _ball->getMeansofMovement() == REST)
	{
		if (!keyPressed)
		{
			hitBall(rotationMatrix, _clubSpeed, _clubChoice);
			keyPressed = true;
		}
	}
	else
		keyPressed = false;
}

DynamicObject * Player::getBall()
{
	return _ball;
}

float Player::getClubSpeed()
{
	return _clubSpeed;
}

int Player::getNrOfHits()
{
	return _nrOfHits;
}

int Player::getPrevNrOfHits()
{
	return _prevNrOfHits;
}


