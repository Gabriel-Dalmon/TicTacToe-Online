#pragma once
#include "PauseGameState.h"

class Window;
class GameObject;

class GameOverGameState : public PauseGameState
{
public:
	GameOverGameState(bool isDefeat, GameState* pausedGameState);
	~GameOverGameState() {};
private:
};