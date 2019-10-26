#include "BoundingSphere.h"
#include "OBB.h"
#include "BoundingPlane.h"
#include <algorithm>

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
		DirectX::XMVECTOR pos, spherePos;
		DirectX::XMFLOAT3 sphere, planeMin, planeMax;
		float x, y, z, distance;
		
		for (float t = 0.0f; t < 1.0f && !info.colliding; t += 0.025f)
		{
			// LERP between previous and current position
			pos = DirectX::XMVectorLerp(getPrevPos(), getPos(), t);
			// Transform sphere to plane's local space
			spherePos = DirectX::XMVector4Transform(pos, DirectX::XMMatrixInverse(nullptr, plane->getWorldMatrix()));
			//// Convert xmvector to xmfloat3
			DirectX::XMStoreFloat3(&sphere, spherePos);
			// Get min and max coordinates
			planeMin = plane->getMinCoordinates();
			planeMax = plane->getMaxCoordinates();

			// Point on plane closest to sphere
			x = max(planeMin.x, min(sphere.x, planeMax.x));
			y = max(planeMin.y, min(sphere.y, planeMax.y));
			z = max(planeMin.z, min(sphere.z, planeMax.z));
		
			// Distance between closest point and sphere (pythagorean theorem)
			x = pow(x - sphere.x, 2);
			y = pow(y - sphere.y, 2);
			z = pow(z - sphere.z, 2);
			distance = sqrt(x + y + z);

			// Collision if sphere radius is longer than shortest distance to plane
			if (distance <= _radius)
			{
				if(t == 0.0f)
					info.pointOfCollision = getPrevPos();
				else
					info.pointOfCollision = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(plane->getNormal(), _radius - distance));

				info.colliding = true;
				info.normal = plane->getNormal();
			}
		}
	}

	return info;
}

CollisionInfo BoundingSphere::intersectsWithOBB(BoundingVolume * other)
{
	CollisionInfo info;
	if (OBB* obb = dynamic_cast<OBB*> (other))
	{
		DirectX::XMVECTOR obbV;
		DirectX::XMFLOAT3 obbF, obbMin, obbMax;
		float x, y, z, distance;

		// LERP between previous and current position
		//pos = DirectX::XMVectorLerp(getPrevPos(), getPos(), t);
		// Transform sphere to obb's local space
		obbV = DirectX::XMVector4Transform(getPos(), DirectX::XMMatrixInverse(nullptr, obb->getWorldMatrix()));
		// Convert xmvector to xmfloat3
		DirectX::XMStoreFloat3(&obbF, obbV);
		// Get min and max coordinates
		obbMin = obb->getMinCoordinates();
		obbMax = obb->getMaxCoordinates();

		// Point on obb closest to sphere
		x = max(obbMin.x, min(obbF.x, obbMax.x));
		y = max(obbMin.y, min(obbF.y, obbMax.y));
		z = max(obbMin.z, min(obbF.z, obbMax.z));

		// Collision normal (vector from closest point on OBB to sphere)
		DirectX::XMVECTOR collisionNormal = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(obbV, DirectX::XMVECTOR{ x, y, z }));

		// Distance between closest point and sphere (pythagorean theorem)
		x = pow(x - obbF.x, 2);
		y = pow(y - obbF.y, 2);
		z = pow(z - obbF.z, 2);
		distance = sqrt(x + y + z);

		// Collision if sphere radius is longer than shortest distance to plane
		if (distance <= _radius + 0.001f)
		{
			info.pointOfCollision = DirectX::XMVectorAdd(getPos(), DirectX::XMVectorScale(collisionNormal, _radius - distance));
			info.colliding = true;
			info.normal = collisionNormal;
		}
	}

	return info;
}
