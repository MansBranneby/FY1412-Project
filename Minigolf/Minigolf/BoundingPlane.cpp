#include "BoundingPlane.h"
#include "OBB.h"
#include "BoundingSphere.h"

BoundingPlane::BoundingPlane(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
	:BoundingVolume(minCoordinates, maxCoordinates)
{
	//Calculate vertices
	// Colour
	DirectX::XMFLOAT3 colour{ 1.0f, 1.0f, 1.0f };

	//Near vertices
	getVertices()->push_back({ maxCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //0. Right near 
	getVertices()->push_back({ maxCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //1. Right far
	getVertices()->push_back({ minCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //2. Left near
	getVertices()->push_back({ minCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //3. left far

	//Indices
	getIndices()->push_back(0); getIndices()->push_back(1); 
	getIndices()->push_back(1); getIndices()->push_back(3);
	getIndices()->push_back(3); getIndices()->push_back(2);
	getIndices()->push_back(2); getIndices()->push_back(0);


	BoundingVolumeVertex vertex1 = getVertices()->at(0);
	BoundingVolumeVertex vertex2 = getVertices()->at(1);
	BoundingVolumeVertex vertex3 = getVertices()->at(2);

	DirectX::XMVECTOR pos1{ vertex1.x, vertex1.y, vertex1.z, 0.0f };
	DirectX::XMVECTOR pos2{ vertex2.x, vertex2.y, vertex2.z, 0.0f };
	DirectX::XMVECTOR pos3{ vertex3.x, vertex3.y, vertex3.z, 0.0f };

	DirectX::XMVECTOR vec1 = DirectX::XMVectorSubtract(pos3, pos1);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSubtract(pos2, pos1);
	_normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vec1, vec2));

	createBuffers(device, *getVertices(), *getIndices());
}

DirectX::XMVECTOR BoundingPlane::getNormal()
{
	/*BoundingVolumeVertex vertex1 = getVertices()->at(0);
	BoundingVolumeVertex vertex2 = getVertices()->at(1);
	BoundingVolumeVertex vertex3 = getVertices()->at(2);
	
	DirectX::XMVECTOR pos1{ vertex1.x, vertex1.y, vertex1.z, 0.0f };
	DirectX::XMVECTOR pos2{ vertex2.x, vertex2.y, vertex2.z, 0.0f };
	DirectX::XMVECTOR pos3{ vertex3.x, vertex3.y, vertex3.z, 0.0f };
	
	DirectX::XMVECTOR vec1 = DirectX::XMVectorSubtract(pos1, pos2);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSubtract(pos1, pos3);*/

	return _normal; //DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vec2, vec1));
}

CollisionInfo BoundingPlane::intersects(BoundingVolume * other)
{
	CollisionInfo info;

	if (dynamic_cast<OBB*> (other))
		info = intersectsWithOBB(other);
	else if (dynamic_cast<BoundingPlane*> (other))
		info = intersectsWithPlane(other);

	return info;
}

CollisionInfo BoundingPlane::intersectsWithPlane(BoundingVolume * other)
{
	CollisionInfo info;
	return info;
}

CollisionInfo BoundingPlane::intersectsWithOBB(BoundingVolume * other)
{
	CollisionInfo info;
	return info;
}
