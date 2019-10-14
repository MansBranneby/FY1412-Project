#include "BoundingSphere.h"
#include "OBB.h"
#include "BoundingPlane.h"

BoundingSphere::BoundingSphere(ID3D11Device * device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
	:BoundingVolume(minCoordinates, maxCoordinates)
{
	_radius = maxCoordinates.x;
}

float BoundingSphere::getRadius()
{
	return _radius;
}

CollisionInfo BoundingSphere::intersects(BoundingVolume * other)
{	
	CollisionInfo info;

	if (dynamic_cast<OBB*> (other))
		info = intersectsWithOBB(other);
	else if (dynamic_cast<BoundingPlane*> (other))
		info = intersectsWithPlane(other);

	return info;
}

CollisionInfo BoundingSphere::intersectsWithPlane(BoundingVolume * other)
{
	CollisionInfo info;
	if (BoundingPlane* plane = dynamic_cast<BoundingPlane*> (other))
	{
		// Transform sphere to plane's local space
		DirectX::XMVECTOR spherePos = DirectX::XMVector3Transform(getPos(), plane->getWorldMatrix());
		// Convert xmvector to xmfloat3
		DirectX::XMFLOAT3 sphere;
		DirectX::XMStoreFloat3(&sphere, spherePos);
		// Get min and max coordinates
		DirectX::XMFLOAT3 planeMin = plane->getMinCoordinates();
		DirectX::XMFLOAT3 planeMax = plane->getMaxCoordinates();

		// Point on plane closest to sphere
		float x = max(planeMin.x, min(sphere.x, planeMax.x));
		float y = max(planeMin.y, min(sphere.y, planeMax.y));
		float z = max(planeMin.z, min(sphere.z, planeMax.z));
		
		// Distance between closest point and sphere (pythagorean theorem)
		x = pow(x - sphere.x, 2);
		y = pow(y - sphere.y, 2);
		z = pow(z - sphere.z, 2);
		float distance = sqrt(x + y + z);

		// Collision if sphere radius is longer than shortest distance to plane
		if (distance <= _radius)
		{
			info.colliding = true;
			info.pointOfCollision = DirectX::operator+(getPos(), DirectX::operator*(_radius - (distance - 0.001f), plane->getNormal()));
		}
	}

	return info;
}

CollisionInfo BoundingSphere::intersectsWithOBB(BoundingVolume * other)
{
	CollisionInfo info;
	return info;
}
