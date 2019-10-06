#pragma once
#include <Keyboard.h>
#include <GamePad.h>
#include "Club.h"
#include "DynamicObject.h"

class Player
{
private:
	Club _club;
	DynamicObject* _geometry;
public:
	Player();
	Player(DynamicObject* geometry);
	virtual ~Player();

	void handleInput(DirectX::Keyboard::State kb, DirectX::GamePad::State gp, float deltaSeconds);
	DynamicObject* getGeometry();
};