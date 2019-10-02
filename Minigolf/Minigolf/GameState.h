#pragma once
#include "State.h"

class GameState : public State
{
private:
	void updatePositions(Game* game, Player* player, UINT32 nrOfObjects);
	void boundingCollision(Game* game, Player* player, UINT32 nrOfObjects);
	void geometryCollision(Game* game, Player* player, UINT32 nrOfObjects);
	void heightmapCalculations(Game* game, Player* player);
public:
	void handleInput(Game* game);
	void update(Game* game);
	void draw(Game* game);
};