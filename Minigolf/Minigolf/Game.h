#pragma once
#include "LevelHandler.h"
#include "Light.h"
#include "State.h"
#include "Clock.h"
#include "InputController.h"
#include "Camera.h"
#include "GraphicResources.h"
#include <Keyboard.h>
#include <vector>

class State; //Forward declare https://stackoverflow.com/questions/6515143/user-created-header-causing-c2061-syntax-error-identifier-classname
class Game
{
private:
	GraphicResources* _graphicResources;
	Camera* _camera = nullptr;
	InputController* _inputController = nullptr; //Input from player
	Clock* _clock = nullptr; //Get delta seconds
	std::vector<State*> _states; //Holds states that should be updated
	LevelHandler* _levelHandler = nullptr;

	Environment* _environment;
	
public:
	Game(Clock* clock, InputController* inputController, GraphicResources* graphicResources);
	~Game();

	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void handleInput();												   //Calls handleInput of the current state
	void update();													   //Calls update on current state
	void draw();													   //Calls draw on current state

	//GET & SET
	GraphicResources* getGraphicResources();
	Clock* getClock();
	InputController* getInputController();
	LevelHandler* getLevelHandler();
	Camera* getCamera();
	Environment* getEnvironment() const;
	
	void updateCameraBuffers();
};

