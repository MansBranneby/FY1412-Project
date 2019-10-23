#pragma once
#include "GameObject.h"

class StaticObject : public GameObject
{
private:

public:
	StaticObject(ObjectType objectType, BoundingType boundingType, DirectX::XMVECTOR startingPosition);
	StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, int goal, DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~StaticObject();

	void updateLogic(); //Otherwise abstract
};