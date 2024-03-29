#include "BoundingVolume.h"


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

DirectX::XMVECTOR BoundingVolume::getPrevPos()
{
	return _prevPos;
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
	_pos = DirectX::XMVectorSetW(pos, 1.0f);
}

void BoundingVolume::setPrevPos(DirectX::XMVECTOR prevPos)
{
	_prevPos = DirectX::XMVectorSetW(prevPos, 1.0f);
}

void BoundingVolume::setWorldMatrix(DirectX::XMMATRIX world)
{
	_world = world;
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