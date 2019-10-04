#pragma once
#include "BoundingPlane.h"
#include "BoundingVolume.h"
class BoundingSphere : public BoundingVolume
{
private:
	float _radius;
	virtual CollisionInfo intersectsWithPlane(BoundingVolume* other);
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other);

public: 
	BoundingSphere(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	float getRadius();

	CollisionInfo intersects(BoundingVolume* other);
};