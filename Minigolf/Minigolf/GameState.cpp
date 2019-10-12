#include "GameState.h"

void GameState::updatePositions(Game * game, Player* player, UINT32 nrOfObjects)
{
	float deltaSeconds = game->getClock()->getDeltaSeconds();
	if(player->getBall()->getMeansofMovement() != REST)
		player->getBall()->setPosition(player->getBall()->calculateMovement(deltaSeconds, game->getEnvironment()));

	for (UINT32 i = 0; i < nrOfObjects; i++)
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
	CollisionInfo colInfo = player->getBall()->getBoundingVolume()->intersects(game->getLevelHandler()->getGameObject(1)->getBoundingVolume());
	if (colInfo.colliding)
	{
		player->getBall()->setPosition(player->getBall()->getPositionVector() + colInfo.pointOfCollision);
		player->getBall()->calculateAfterColVel(game->getLevelHandler()->getGameObject(1));
	}
		//player->getGeometry()->setMeansOfMovement(MeansOfMovement(REST));

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
	DirectX::Mouse::State ms = game->getInputController()->getMouseState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	Player* player = game->getLevelHandler()->getPlayer();
	Camera* camera = game->getCamera();
	
	game->getInputController()->setMouseMode();
	// Handle player
	player->handleInput(kb, gp, ms, deltaSeconds);
	// Handle camera
	camera->handleInput(kb, ms, deltaSeconds, player->getBall()->getPositionVector());
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
}

void GameState::draw(Game* game)
{
	// Draw active level. 
	// A level may contain terrain, objects and a player
	game->getLevelHandler()->drawLevel(game->getGraphicResources(), game->getClock()->getTimeInSec());      
}
