#include "StaticObject.h"

StaticObject::StaticObject(ObjectType objectType, BoundingType boundingType, DirectX::XMVECTOR startingPosition)
	:GameObject(objectType, BoundingType(boundingType), startingPosition)
{
}

StaticObject::StaticObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:GameObject(device, deviceContext, STATICOBJECT, BoundingType(boundingType), startingPosition, modelFile)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::updateLogic()
{
}
