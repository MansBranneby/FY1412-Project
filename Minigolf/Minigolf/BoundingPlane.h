#pragma once
#include "BoundingVolume.h"
class BoundingPlane : public BoundingVolume
{
private:
	CollisionInfo intersectsWithPlane(BoundingVolume* other);
	CollisionInfo intersectsWithOBB(BoundingVolume* other);
public:
	BoundingPlane(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);

	// return normalized normal
	DirectX::XMVECTOR getNormal();

	CollisionInfo intersects(BoundingVolume* other);
};