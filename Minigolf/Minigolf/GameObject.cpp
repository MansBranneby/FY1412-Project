#include "GameObject.h"

void GameObject::updateTransformations(XMFLOAT3 position)
{
	_model.updateTransformation(position);
}

void GameObject::updateTransformations(XMFLOAT3 position, XMFLOAT3 rotation)
{
	aiQuaternion aiRotation = aiQuaternion(rotation.x, rotation.y, rotation.z);
	_model.updateTransformation(position, aiRotation);
}

GameObject::GameObject(ObjectType objectType, BoundingType boundingType, DirectX::XMVECTOR startingPosition)
{
	_position = startingPosition;
	_objectType = objectType;
	_boundingType = boundingType;
}

GameObject::GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, BoundingType boundingType, DirectX::XMVECTOR position, std::string modelFile)
{
	_position = position;
	_boundingType = boundingType;
	createModel(device, deviceContext, modelFile);
} 

GameObject::~GameObject()
{
}

Model * GameObject::getModel()
{
	return &_model;
}

bool GameObject::createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFile)
{
	switch (_objectType)
	{
	case DYNAMICOBJECT:
		_model.loadModel(device, deviceContext, _modelDirectory + modelFile, _boundingType);
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
		_model.getBoundingVolume()->setPrevPos(getBoundingVolume()->getPos() + _position);
		_model.getBoundingVolume()->setPos(getBoundingVolume()->getPos() + _position);

		break;
	case STATICOBJECT:
		_model.loadModel(device, deviceContext, _modelDirectory + modelFile, _boundingType);
		_model.updateTransformation(XMFLOAT3(XMVectorGetX(_position), XMVectorGetY(_position), XMVectorGetZ(_position)));
		_model.getBoundingVolume()->setPrevPos(getBoundingVolume()->getPos() + _position);
		_model.getBoundingVolume()->setPos(_position);
		break;
	default:
		break;
	};
	return true;
}

void GameObject::draw(GraphicResources* graphicResources, float timeInSec)
{
	_model.draw(graphicResources, timeInSec);
}

void GameObject::draw(GraphicResources* graphicResources)
{
	_model.draw(graphicResources);
}

DirectX::XMVECTOR GameObject::getPrevPos()
{
	return _prevPos;
}
void GameObject::drawBoundingVolume(GraphicResources* graphicResources)
{
	_model.drawBoundingVolume(graphicResources);
}

XMVECTOR GameObject::getPositionVector()
{
	return _position;
}

DirectX::XMFLOAT3 GameObject::getPositionFloat3()
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, _position);

	return position;
}

DirectX::XMVECTOR GameObject::getRotation()
{
	return _model.getRotation();
}

void GameObject::setPosition(DirectX::XMVECTOR position)
{
	_position = position;
	setBoundingPos(position);
}

void GameObject::setPrevPos(DirectX::XMVECTOR prevPos)
{
	_prevPos = prevPos;
	setBoundingPrevPos(prevPos);
}

void GameObject::setHeight(float height)
{
	_position = DirectX::XMVectorSet(DirectX::XMVectorGetX(_position), height, DirectX::XMVectorGetZ(_position), 1.0f);
	getBoundingVolume()->setPos(DirectX::XMVectorSet(DirectX::XMVectorGetX(_position), height, DirectX::XMVectorGetZ(_position), 1.0f));
}

BoundingVolume * GameObject::getBoundingVolume()
{
	return _model.getBoundingVolume();
}

void GameObject::setBoundingPos(XMVECTOR position)
{
	_model.getBoundingVolume()->setPos(position);
}

void GameObject::setBoundingPrevPos(XMVECTOR prevPositon)
{
	_model.getBoundingVolume()->setPrevPos(prevPositon);
}

BoundingType GameObject::getBoundingType() const
{
	return _boundingType;
}

ObjectType GameObject::getObjectType() const
{
	return _objectType;
}

void GameObject::setObjectType(ObjectType objectType)
{
	_objectType = objectType;
}
