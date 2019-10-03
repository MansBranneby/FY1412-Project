#pragma once
#include "BoundingVolume.h"
class BoundingPlane : public BoundingVolume
{

public:
	BoundingPlane(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);

	DirectX::XMVECTOR getNormal();
	
	CollisionInfo intersectsWithPlane(BoundingVolume* other);
	CollisionInfo intersectsWithOBB(BoundingVolume* other);
};