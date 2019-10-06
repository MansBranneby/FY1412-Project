#pragma once
#include "DynamicObject.h"
#include "Club.h"

#include <Keyboard.h>
#include <GamePad.h>

class Player : public DynamicObject
{
private:
	Club _club;

public:
	Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~Player();

	void handleInput(Keyboard::State kb, GamePad::State gp, float deltaSeconds);
};