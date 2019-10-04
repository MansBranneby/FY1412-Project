#include "GameState.h"

void GameState::updatePositions(Game * game, Player* player, UINT32 nrOfObjects)
{
	float deltaSeconds = game->getClock()->getDeltaSeconds();
	player->setPosition(player->calculateMovement(deltaSeconds)); //OBS. Var uppdateras boundingBoxes position

	for (int i = 0; i < nrOfObjects; i++)
	{
		//Update postion of all dynamic objects
	}
}

//void GameState::boundingCollision(Game* game, Player* player, UINT32 nrOfObjects)
//{
//	// currently just moves player in the opposite direction until no collision. This also prevents player from "gliding along the surface" which isn't good.
//	// In order to fix not being able to slide along the surface one has to find the collision normal and push out that way
//
//	bool colliding = false;
//
//	for (int i = 0; i < nrOfObjects && colliding == false; i++)
//		colliding = player->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()).colliding; //Not safe, will crasch if you try to access gameobject outside of array.		
//
//	// if collision between player and object change travel direction
//	if (colliding)
//	{
//		player->move(-player->getAcceleration(), game->getClock()->getDeltaSeconds());
//	}
//	else
//		player->move(player->getAcceleration(), game->getClock()->getDeltaSeconds());
//}

void GameState::geometryCollision(Game* game, Player* player, UINT32 nrOfObjects)
{
	// SPHERE VS PLANE
	if (player->getBoundingVolume()->intersectsWithPlane(game->getLevelHandler()->getGameObject(0)->getBoundingVolume()).colliding == false)
	{
		//player->height -= 9.82f * game->getClock()->getDeltaSeconds() * 4;
		//player->setHeight(player->height);
	}
	//if collision
		//if N > mg
			//player->calculatenewVel(game, collidingObj)
		//else
			//set gliding(MYgliding)

}

void GameState::heightmapCalculations(Game * game, Player * player)
{
	/*
	// Set player height to terrain
	Terrain* terrain = game->getLevelHandler()->getTerrain(0); // currently only works for one terrain
	float height = terrain->getHeight(player->getPositionFloat3().x, player->getPositionFloat3().z);
	player->setHeight(height);
	*/
}

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	
	// Check for collision, if there is collision then we don't want to allow the player to move until it's resolved (pushed out of collision)
	 //Not safe, will crasch if you try to access gameobject outside of array.
	bool colliding = false;
	Player* player = game->getLevelHandler()->getPlayer();
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();

	for (int i = 0; i < nrOfObjects && colliding == false; ++i)
		colliding = player->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()).colliding;

	// Handle input if there is no collison
	if (colliding == false)
	{
		player->handleInput(kb, gp, deltaSeconds);
	}
	
}

void GameState::update(Game* game)
{
	game->updateCameraBuffers();
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	Player* player = game->getLevelHandler()->getPlayer();

	//Position update
	updatePositions(game, player, nrOfObjects);

	//Collision
	//boundingCollision(game, player, nrOfObjects); //For later use
	geometryCollision(game, player, nrOfObjects);

	//Heightmap calculations
	//heightmapCalculations(game, player);

	//Update Camera
	game->getCamera()->followObject(player->getPositionVector(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	// Draw active level. 
	// A level may contain terrain, objects and a player
	game->getLevelHandler()->drawLevel(game->getGraphicResources(), game->getClock()->getTimeInSec());      
}
