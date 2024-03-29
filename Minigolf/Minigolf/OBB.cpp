#include "OBB.h"
#include "BoundingPlane.h"
#include "BoundingSphere.h"

OBB::OBB(ID3D11Device* device, DirectX::XMFLOAT3 minCoordinates, DirectX::XMFLOAT3 maxCoordinates)
	:BoundingVolume(minCoordinates, maxCoordinates)
{
	using namespace DirectX;

	//Local axes
	_xAxis = { 1.0f, 0.0f, 0.0f, 0.0f };
	_yAxis = { 0.0f, 1.0f, 0.0f, 0.0f };
	_zAxis = { 0.0f, 0.0f, 1.0f, 0.0f };
	_axes.push_back(_xAxis);
	_axes.push_back(_yAxis);
	_axes.push_back(_zAxis);
	_axes.push_back(-_xAxis);
	_axes.push_back(-_yAxis);
	_axes.push_back(-_zAxis);
	//Half width, height and length
	_halfXYZ =
	{
		(maxCoordinates.x - minCoordinates.x) / 4,
		(maxCoordinates.y - minCoordinates.y) / 4,
		(maxCoordinates.z - minCoordinates.z) / 4,
	};

	//Calculate vertices

	// Colour
	XMFLOAT3 colour{ 1.0f, 1.0f, 1.0f };

	// Make the drawn bounding volume a bit bigger to prevent overlapping
	// This will only be visual and won't actually affect the collision
	maxCoordinates.x += 0.02f;
	maxCoordinates.y += 0.02f;
	maxCoordinates.z += 0.02f;
	minCoordinates.x -= 0.02f;
	minCoordinates.y -= 0.02f;
	minCoordinates.z -= 0.02f;

	//Near vertices
	getVertices()->push_back({ maxCoordinates.x, maxCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //0. Right up near 
	getVertices()->push_back({ maxCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //1. Right down near
	getVertices()->push_back({ minCoordinates.x, maxCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //2. Left up near
	getVertices()->push_back({ minCoordinates.x, minCoordinates.y, minCoordinates.z, colour.x, colour.y, colour.z }); //3. left down near

	//Far vertices
	getVertices()->push_back({ maxCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //4. Right up far
	getVertices()->push_back({ maxCoordinates.x, minCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //5. Right down far
	getVertices()->push_back({ minCoordinates.x, maxCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //6. Left up far
	getVertices()->push_back({ minCoordinates.x, minCoordinates.y, maxCoordinates.z, colour.x, colour.y, colour.z }); //7. left down far 

	//Indices
	getIndices()->push_back(0); getIndices()->push_back(1); //RUN -> RDN
	getIndices()->push_back(1); getIndices()->push_back(3); //RDN -> LDN
	getIndices()->push_back(3); getIndices()->push_back(2); //LDN -> LUN
	getIndices()->push_back(2); getIndices()->push_back(0); //LUN -> RUN
	getIndices()->push_back(0); getIndices()->push_back(4); //RUN -> RUF
	getIndices()->push_back(4); getIndices()->push_back(5); //RUF -> RDF
	getIndices()->push_back(5); getIndices()->push_back(1); //RDF -> RDN
	getIndices()->push_back(2); getIndices()->push_back(6); //LUN -> LUF
	getIndices()->push_back(6); getIndices()->push_back(7); //LUF -> LDF
	getIndices()->push_back(7); getIndices()->push_back(3); //LDF -> LDN
	getIndices()->push_back(6); getIndices()->push_back(4); //LUF -> RUF 
	getIndices()->push_back(7); getIndices()->push_back(5); //LDF -> RDF

	createBuffers(device, *getVertices(), *getIndices());
}

CollisionInfo OBB::intersectsWithPlane(BoundingVolume * other)
{
	CollisionInfo info;
	return info;
}

CollisionInfo OBB::intersectsWithOBB(BoundingVolume * other)
{
	using namespace DirectX;
	// dynamic_cast is useful when you don't know what the dynamic type of the object is.
	//It returns a null pointer if the object referred to doesn't contain the type casted to as a base class 
	CollisionInfo collInfo;
	if(OBB* otherOBB = dynamic_cast<OBB*> (other))
	{
		// Convert otherOBB center to this local space
		XMMATRIX inverseWorldMatrix = XMMatrixInverse(nullptr, getWorldMatrix());
		XMVECTOR otherCenter = XMVector4Transform(otherOBB->getPos(), inverseWorldMatrix);
		XMVECTOR vectorToOBB = otherCenter - getPos();

		// Testing face normals and edge combinations
		// 3 * 2 = 6 normal faces
		// 3 * 3 = 9 edge combinations
		// Total = 15 tests

		// Face normals of this OBB
		// Seperating axis (SA) = _xAxis
		float projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _xAxis))));
		float boxProjection = _halfXYZ.x +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_zAxis)))));
		if ((projection/2) > boxProjection)
			return collInfo;
		// 2. SA = _yAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _yAxis))));
		boxProjection = _halfXYZ.y +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_zAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		// 3. SA = _zAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, _zAxis))));
		boxProjection = _halfXYZ.z +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_zAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;

		// Face normals of other OBB
		// 4. SA = xAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_xAxis))));
		boxProjection = otherOBB->_halfXYZ.x +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		// 5. SA = yAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_yAxis))));
		boxProjection = otherOBB->_halfXYZ.y +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_yAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		// 6. SA = zAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, otherOBB->_zAxis))));
		boxProjection = otherOBB->_halfXYZ.z +
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_zAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;

		// All edge combinations
		// 7. SA = _xAxis x xAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_xAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_yAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		// 8. SA = _xAxis x yAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_yAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//9. SA = _xAxis x zAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_xAxis, otherOBB->_zAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//10. SA = _yAxis x xAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_xAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_yAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//11. SA = _yAxis x yAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_yAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_yAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//12. SA = _yAxis x zAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_yAxis, otherOBB->_zAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.z*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_yAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//13. SA = _zAxis x xAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_xAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_xAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_yAxis)))));
		if((projection / 2) > boxProjection)
			return collInfo;
		//14. SA = _zAxis x yAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_yAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.z*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
		//15. SA = _zAxis x zAxis
		projection = abs(XMVectorGetX((XMVector3Dot(vectorToOBB, XMVector3Cross(_zAxis, otherOBB->_zAxis)))));
		boxProjection =
			abs(XMVectorGetX(((_halfXYZ.x*XMVector3Dot(_yAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((_halfXYZ.y*XMVector3Dot(_xAxis, otherOBB->_zAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.x*XMVector3Dot(_zAxis, otherOBB->_yAxis))))) +
			abs(XMVectorGetX(((otherOBB->_halfXYZ.y*XMVector3Dot(_zAxis, otherOBB->_xAxis)))));
		if ((projection / 2) > boxProjection)
			return collInfo;
	}


	collInfo.colliding = true;
	return collInfo;;
}

CollisionInfo OBB::intersects(BoundingVolume * other)
{
	CollisionInfo info;

	if (dynamic_cast<OBB*> (other))
		info = intersectsWithOBB(other);
	else if (dynamic_cast<BoundingPlane*> (other))
		info = intersectsWithPlane(other);

	return info;
}

std::vector<DirectX::XMVECTOR> OBB::getAxes()
{
	return _axes;
}
