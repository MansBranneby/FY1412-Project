#pragma once
#include <Keyboard.h>
#include <GamePad.h>
#include <Mouse.h>
#include "DynamicObject.h"

struct Club
{
	float mass;
	float loft;
};

enum ClubType {WOOD_1, WOOD_3, WOOD_5, IRON_2, IRON_3, IRON_4, IRON_5, IRON_6, IRON_7, IRON_8, IRON_9, P_WEDGE, S_WEDGE, PUTTER};

class Player
{
private:
	Club _clubs[14] = { {0.2f, 11.0f * XM_PI / 180}, {0.208f, 15.0f * XM_PI / 180}, {0.218f, 18.0f * XM_PI / 180}, {0.232f, 18.0f * XM_PI / 180}, {0.239f, 21.0f * XM_PI / 180}, {0.246f, 24.0f * XM_PI / 180}, {0.253f, 27.0f * XM_PI / 180}, {0.260f, 31.0f * XM_PI / 180}, {0.267f, 35.0f * XM_PI / 180}, {0.274f, 39.0f * XM_PI / 180}, {0.281f, 43.0f * XM_PI / 180}, {0.285f, 48.0f * XM_PI / 180}, {0.296f, 55.0f * XM_PI / 180}, {0.33f, 4.0f * XM_PI / 180} };
	DynamicObject* _ball;
	float _yRotation;
	float _clubSpeed;
	float _clubSpeedIncrement;
	ClubType _clubChoice;

	// Game variables
	int _nrOfHits, _prevNrOfHits;
	bool keyPressed;
	void hitBall(XMMATRIX rotationMat, float clubSpeed, ClubType clubType);

public:
	Player();
	Player(DynamicObject* ball);
	virtual ~Player();

	void setClubChoice(ClubType clubChoice);
	void setPrevNrOfHits(int prevNrOfHits);
	void setNrOfHits(int nrOfHits);

	DynamicObject* getBall();
	float getClubSpeed();
	int getNrOfHits();
	int getPrevNrOfHits();
	void handleInput(DirectX::Keyboard::State kb, DirectX::GamePad::State gp, DirectX::Mouse::State ms, float deltaSeconds);

};