#include "StaticObject.h"

StaticObject::StaticObject(ObjectType objectType, Geometry geometryType,DirectX::XMVECTOR startingPosition)
	:GameObject(objectType, Geometry(geometryType),startingPosition)
{
}

StaticObject::StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Geometry geometryType,DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, STATICOBJECT, Geometry(geometryType),startingPosition, modelFile)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::updateLogic()
{
}
