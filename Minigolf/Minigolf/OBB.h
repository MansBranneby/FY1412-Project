#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	// Local axes
	DirectX::XMVECTOR _xAxis;
	DirectX::XMVECTOR _yAxis;
	DirectX::XMVECTOR _zAxis;
	std::vector<DirectX::XMVECTOR> _axes;
	// Half with, height and length
	DirectX::XMFLOAT3 _halfXYZ;

	CollisionInfo intersectsWithPlane(BoundingVolume* other);
	CollisionInfo intersectsWithOBB(BoundingVolume* other);

public:
	OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	CollisionInfo intersects(BoundingVolume* other);
	std::vector<DirectX::XMVECTOR> getAxes();
};