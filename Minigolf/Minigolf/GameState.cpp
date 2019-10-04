#include "GameState.h"

void GameState::updatePositions(Game * game, Player* player, UINT32 nrOfObjects)
{
	float deltaSeconds = game->getClock()->getDeltaSeconds();
	//player->setPosition(player->calculateMovement(deltaSeconds));

	//player->setPosition(player->getPositionVector() + (player->getVelocity() * deltaSeconds));

	for (int i = 0; i < nrOfObjects; i++)
	{
		//Update postion of all dynamic objects
	}
}

void GameState::boundingCollision(Game* game, Player* player, UINT32 nrOfObjects)
{
}

void GameState::geometryCollision(Game* game, Player* player, UINT32 nrOfObjects)
{
	// SPHERE VS PLANE
	player->move(player->getAcceleration(), game->getClock()->getDeltaSeconds());
	if (player->getBoundingVolume()->intersects(game->getLevelHandler()->getGameObject(0)->getBoundingVolume()).colliding == false)
	{
		player->height -= 9.82f * game->getClock()->getDeltaSeconds() * 4;
		player->setHeight(player->height);
	}
	//if collision
		//if N > mg
			//player->calculatenewVel(game, collidingObj)
		//else
			//set gliding(MYgliding)

}

void GameState::heightmapCalculations(Game * game, Player * player)
{
}

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	Player* player = game->getLevelHandler()->getPlayer();

	// Handle input if there is no collison
	player->handleInput(kb, gp, deltaSeconds);
}

void GameState::update(Game* game)
{
	game->updateCameraBuffers();
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	Player* player = game->getLevelHandler()->getPlayer();

	//Position update
	updatePositions(game, player, nrOfObjects);

	//Collision
	geometryCollision(game, player, nrOfObjects);

	//Update Camera
	game->getCamera()->followObject(player->getPositionVector(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	// Draw active level. 
	// A level may contain terrain, objects and a player
	game->getLevelHandler()->drawLevel(game->getGraphicResources(), game->getClock()->getTimeInSec());      
}
