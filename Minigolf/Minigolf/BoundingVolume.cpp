#include "BoundingVolume.h"

void BoundingVolume::createBuffers(ID3D11Device * device, std::vector<BoundingVolumeVertex> vertices, std::vector<int> indices)
{
	//CREATE VERTEX BUFFER
	D3D11_BUFFER_DESC vdesc;
	vdesc.Usage = D3D11_USAGE_IMMUTABLE;
	vdesc.ByteWidth = sizeof(BoundingVolumeVertex) * (UINT)vertices.size();
	vdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vdesc.CPUAccessFlags = 0;
	vdesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	HRESULT hr = device->CreateBuffer(&vdesc, &initData, &_vertexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error BoundingVolume VertexBuffer", L"Error", MB_OK | MB_ICONERROR);

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC idesc;
	idesc.Usage = D3D11_USAGE_IMMUTABLE;
	idesc.ByteWidth = sizeof(int) * (UINT)indices.size();
	idesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idesc.CPUAccessFlags = 0;
	idesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	hr = device->CreateBuffer(&idesc, &initData, &_indexBuffer);
	if (FAILED(hr))
		MessageBox(NULL, L"Error BoundingVolume Index Buffer", L"Error", MB_OK | MB_ICONERROR);
}

BoundingVolume::BoundingVolume()
{
}

BoundingVolume::BoundingVolume(DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
{
	//Center which is the same as position
	_minCoordinates = minCoordinates;
	_maxCoordinates = maxCoordinates;
	_pos =
	{
		(maxCoordinates.x + minCoordinates.x) / 2,
		(maxCoordinates.y + minCoordinates.y) / 2,
		(maxCoordinates.z + minCoordinates.z) / 2,
		1.0f
	};
}

BoundingVolume::~BoundingVolume()
{

	if(_vertexBuffer)
		_vertexBuffer->Release();
	if(_indexBuffer)
		_indexBuffer->Release();
}

DirectX::XMVECTOR BoundingVolume::getPos()
{
	return _pos;
}

DirectX::XMMATRIX BoundingVolume::getWorldMatrix()
{
	return _world;
}

std::vector<BoundingVolumeVertex>* BoundingVolume::getVertices()
{
	return &_vertices;
}

std::vector<int>* BoundingVolume::getIndices()
{
	return &_indices;
}

void BoundingVolume::setPos(DirectX::XMVECTOR pos)
{
	_pos = pos;
}

void BoundingVolume::move(DirectX::XMVECTOR speed)
{
	_pos = DirectX::XMVectorAdd(_pos, speed);
}

void BoundingVolume::setWorldMatrix(aiMatrix4x4 world)
{	
	_world = DirectX::XMMATRIX(DirectX::XMVECTOR{world.a1, world.a2, world.a3, world.a4}, 
							   DirectX::XMVECTOR{world.b1, world.b2, world.b3, world.b4}, 
		                       DirectX::XMVECTOR{world.c1, world.c2, world.c3, world.c4}, 
					           DirectX::XMVECTOR{world.d1, world.d2, world.d3, world.d4});
	for (UINT32 i = 0; i < _vertices.size(); ++i)
	{
		DirectX::XMVECTOR pos{ _vertices[i].x, _vertices[i].y, _vertices[i].z};
		pos = DirectX::XMVector3Transform(pos, DirectX::XMMatrixTranspose(_world));
		_vertices[i].x = DirectX::XMVectorGetX(pos);
		_vertices[i].y = DirectX::XMVectorGetY(pos);
		_vertices[i].z = DirectX::XMVectorGetZ(pos);
	}
}

DirectX::XMFLOAT3 BoundingVolume::getMinCoordinates()
{
	return _minCoordinates;
}

DirectX::XMFLOAT3 BoundingVolume::getMaxCoordinates()
{
	return _maxCoordinates;
}

void BoundingVolume::draw(GraphicResources* graphicResources)
{
	UINT offset = 0;
	UINT vertexSize = sizeof(BoundingVolumeVertex);

	graphicResources->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	graphicResources->getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	graphicResources->getDeviceContext()->DrawIndexed((UINT)_indices.size(), 0, 0);
}
