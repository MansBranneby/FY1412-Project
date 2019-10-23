#pragma once
#include "Model.h"
#include "GraphicResources.h"
using namespace DirectX;

enum ObjectType //Troligtvis ett tillfälligt sätt att hantera detta.
{
	DYNAMICOBJECT,
	STATICOBJECT
};

class GameObject
{
private:
	Model _model;
	ObjectType _objectType;
	BoundingType _boundingType;
	std::string _modelDirectory = ".\\Resources\\Models\\";
	XMVECTOR _position, _prevPos;

	// Game variables
	bool _cup;
	
protected:
	XMVECTOR _lookAt;
	void updateTransformations(XMFLOAT3 position);
	void updateTransformations(DirectX::XMMATRIX world);

public:
	GameObject(ObjectType objectType, BoundingType _boundingType, DirectX::XMVECTOR startingPosition);
	GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ObjectType objectType, BoundingType boundingType, DirectX::XMVECTOR position, std::string modelFile);
	virtual ~GameObject() = 0;

	Model* getModel();
	bool createModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFile); //Return true if everything works out
	void draw(GraphicResources* graphicResources);
	void draw(GraphicResources* graphicResources, float timeInSec);
	void drawBoundingVolume(GraphicResources* graphicResources);

	//GET & SET
	virtual void setPosition(DirectX::XMVECTOR position);
	virtual void setPrevPos(DirectX::XMVECTOR prevPos);
	void setHeight(float height);

	virtual DirectX::XMVECTOR getPrevPos();
	virtual	DirectX::XMVECTOR getPositionVector();
	DirectX::XMFLOAT3 getPositionFloat3();
	DirectX::XMVECTOR getRotation();

	virtual BoundingVolume* getBoundingVolume();
	virtual void setBoundingPos(XMVECTOR positon);
	virtual void setBoundingPrevPos(XMVECTOR prevPositon);

	BoundingType getBoundingType() const;
	ObjectType getObjectType() const;
	void setObjectType(ObjectType objectType);
	void setCup(bool cup);
	bool getCup();
};