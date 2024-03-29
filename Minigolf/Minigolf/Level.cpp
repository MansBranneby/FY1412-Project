#include "Level.h"

Level::Level(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int levelNr)
{
	_objHandler = new GameObjectHandler(device, deviceContext);
	_levelAI.createLevel(levelNr, _objHandler);
}

Level::~Level()
{
	delete _objHandler;
}

void Level::drawLevel(GraphicResources* graphicResources, float timeInSec)
{
	graphicResources->getDeviceContext()->VSSetShader(&graphicResources->getShaderHandler()->getObjectVS()->getVertexShader(), nullptr, 0);
	graphicResources->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->PSSetShader(&graphicResources->getShaderHandler()->getObjectPS()->getPixelShader(), nullptr, 0);

	graphicResources->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicResources->getDeviceContext()->IASetInputLayout(&graphicResources->getShaderHandler()->getObjectVS()->getVertexLayout());

	_objHandler->drawObjects(graphicResources);
	
	graphicResources->getDeviceContext()->VSSetShader(&graphicResources->getShaderHandler()->getTerrainVS()->getVertexShader(), nullptr, 0);
	graphicResources->getDeviceContext()->HSSetShader(&graphicResources->getShaderHandler()->getTerrainHS()->getHullShader() , nullptr, 0);
	graphicResources->getDeviceContext()->DSSetShader(&graphicResources->getShaderHandler()->getTerrainDS()->getDomainShader(), nullptr, 0);
	graphicResources->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	graphicResources->getDeviceContext()->PSSetShader(&graphicResources->getShaderHandler()->getTerrainPS()->getPixelShader(), nullptr, 0);

	graphicResources->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	graphicResources->getDeviceContext()->IASetInputLayout(&graphicResources->getShaderHandler()->getTerrainVS()->getVertexLayout());

	_objHandler->drawTerrain(graphicResources);
}

int Level::getNrOfGameObjects() const
{
	return _objHandler->getNrOfObjects();
}

GameObject * Level::getGameObject(int index)
{
	return _objHandler->getGameObject(index);
}

Terrain * Level::getTerrain(int index)
{
	return _objHandler->getTerrain()[index];
}

Player * Level::getPlayer()
{
	return _objHandler->getPlayer();
}
