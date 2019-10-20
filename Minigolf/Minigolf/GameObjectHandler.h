#pragma once
#include "Player.h"
#include "StaticObject.h"
#include "Sphere.h"
#include "Terrain.h"
#include "GraphicResources.h"

class GameObjectHandler
{
private:
	//Graphics
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;

	//Container
	int _capacity;
	int _nrOfObjects;
	GameObject** _gameObjects = nullptr;
	std::vector<Terrain*> _terrain; // vector filled with terrain
	Player* _player;

	//Manage container
	void freeMemory();
	void expand();
	void initiate(int from = 0);
public:
	GameObjectHandler(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int capacity = 10);
	virtual~GameObjectHandler();

	void addGameObject(ObjectType objType, BoundingType boundingType, XMVECTOR position, std::string modelFile);
	void addPlayer();
	int getNrOfObjects() const;
	std::vector<Terrain*> getTerrain();
	Player* getPlayer();

	void drawTerrain(GraphicResources* graphicResources);
	void drawObjects(GraphicResources* graphicResources);
	void drawObjects(GraphicResources* graphicResources, float timeInSec);

	GameObject* getGameObject(int index) const;
};