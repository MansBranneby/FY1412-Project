#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(ID3D11Device * device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
	:BoundingVolume(minCoordinates, maxCoordinates)
{
	_radius = maxCoordinates.x;
}

float BoundingSphere::getRadius()
{
	return _radius;
}

CollisionInfo BoundingSphere::intersectsWithPlane(BoundingVolume * other)
{
	CollisionInfo info;
	if (BoundingPlane* plane = dynamic_cast<BoundingPlane*> (other))
	{
		DirectX::XMVECTOR planeToSphere = DirectX::XMVectorSubtract(getPos(), plane->getPos());
		DirectX::XMVECTOR planeNormal = plane->getNormal();

		float distanceToPlane = DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeToSphere, planeNormal));

		if (distanceToPlane <= _radius)
			info.colliding = true;
	}
	return info;

}

CollisionInfo BoundingSphere::intersectsWithOBB(BoundingVolume * other)
{
	CollisionInfo info;
	return info;
}
