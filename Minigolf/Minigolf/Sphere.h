#pragma once
#include "DynamicObject.h"

class Sphere : public DynamicObject
{
private:
	float _radius;

	void planeCol(GameObject* colObj);
	void obbCol(GameObject* colObj, XMVECTOR normal);
public:
	Sphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile);

	XMVECTOR calculateDrag(Environment* environment);
	void calculateAfterColVel(GameObject* colObj, XMVECTOR normal);
};