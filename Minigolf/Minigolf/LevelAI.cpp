#include "LevelAI.h"

LevelAI::LevelAI()
{
}

LevelAI::~LevelAI()
{
}

void LevelAI::createLevel(int levelNr, GameObjectHandler * objHandler)
{
	//Read from file: ObjectType, position, modelfile.
	//Add GameObjects based on file.
	std::string line;
	std::ifstream infile;
	switch (levelNr)
	{
	case 1:
	{
		infile = std::ifstream(".\\Resources\\Levels\\level3.txt");
		break;
	}
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		int objType, boundingType, goal;
		float x, y, z;
		std::string modelFile;
		if (!(iss >> objType >> boundingType >> goal >> x >> y >> z >> modelFile)) { break; }
		objHandler->addGameObject(ObjectType(objType), BoundingType(boundingType), goal, XMVectorSet(x, y, z, 1.0f), modelFile);
	}

	objHandler->addPlayer();
}
