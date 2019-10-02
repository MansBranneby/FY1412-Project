#pragma once
#include "GameObject.h"

class StaticObject : public GameObject
{
private:

public:
	StaticObject(ObjectType objectType, Geometry geometryType,DirectX::XMVECTOR startingPosition);
	StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Geometry geometryType,DirectX::XMVECTOR startingPosition, std::string modelFile);
	virtual ~StaticObject();

	void updateLogic(); //Otherwise abstract
};