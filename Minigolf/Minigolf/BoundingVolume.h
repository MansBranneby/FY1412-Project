#pragma once
#include "GraphicResources.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

enum BoundingType { BOUNDING_SPHERE, BOUNDING_PLANE, ORIENTED_BOUNDING_BOX };

struct BoundingVolumeVertex
{
	float x, y, z;
	float r, g, b;
};

// Information acquired from collision detection
struct CollisionInfo
{
	bool colliding;

	CollisionInfo()
	{
		colliding = false;
	}
};

class BoundingVolume
{
private:
	std::vector<BoundingVolumeVertex> _vertices;
	std::vector<int> _indices;
	DirectX::XMMATRIX _world;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	DirectX::XMVECTOR _pos;

	bool drawBoundingVolume;

protected:
	virtual void createBuffers(ID3D11Device* device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices);

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates);
	virtual ~BoundingVolume();
	virtual DirectX::XMVECTOR getPos();
	DirectX::XMMATRIX getWorldMatrix();
	std::vector<BoundingVolumeVertex>* getVertices();
	std::vector<int>* getIndices();

	virtual void setPos(DirectX::XMVECTOR pos);
	virtual void move(DirectX::XMVECTOR speed);

	virtual void draw(GraphicResources* graphicResources);
	
	virtual CollisionInfo intersectsWithPlane(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other) = 0;
};