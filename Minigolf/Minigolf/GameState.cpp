#include "GameState.h"

void GameState::updatePositions(Game * game, Player* player, UINT32 nrOfObjects)
{
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	if (player->getBall()->getMeansofMovement() != REST)
	{
		player->getBall()->setPosition(player->getBall()->calculateMovement(deltaSeconds, game->getEnvironment()));
	}

	//for (UINT32 i = 0; i < nrOfObjects; i++)
	//{
	//	//Update postion of all dynamic objects
	//}
}

void GameState::boundingCollision(Game* game, Player* player, UINT32 nrOfObjects)
{
}

void GameState::geometryCollision(Game* game, Player* player, UINT32 nrOfObjects)
{
	bool isColliding = false;
	for (UINT32 i = 1; i < nrOfObjects; ++i)
	{
		CollisionInfo colInfo = player->getBall()->getBoundingVolume()->intersects(game->getLevelHandler()->getGameObject(i)->getBoundingVolume());
		if (colInfo.colliding)
		{
 			if(game->getLevelHandler()->getGameObject(i)->getCup())
			{
				player->setPrevNrOfHits(player->getNrOfHits());
				player->setNrOfHits(0);
				player->getBall()->setVelocity(DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f });
				player->getBall()->setAngularVelocity(DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f });
				player->getBall()->setPrevPos(DirectX::XMVECTOR{ -1.0f, 10.0f, 1.0f });
				player->getBall()->setPosition(DirectX::XMVECTOR{ -1.0f, 10.0f, 1.0f });
				player->getBall()->setMeansOfMovement(MeansOfMovement(PROJECTILE));
			}
			else
			{
				isColliding = true;
				player->getBall()->setPosition(colInfo.pointOfCollision);
				float typeOfCollision = XMVectorGetX(XMVector3Dot(player->getBall()->getVelocity(), colInfo.normal));

				if (typeOfCollision < 0.0f)
					player->getBall()->calculateAfterColVel(game->getLevelHandler()->getGameObject(i), colInfo.normal);	
			}

		}
	}
	if (isColliding == false)
	{
		player->getBall()->setSurfaceNormal(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		player->getBall()->setMeansOfMovement(MeansOfMovement(PROJECTILE));
		player->getBall()->setPrevPos(player->getBall()->getPositionVector());
	}
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
