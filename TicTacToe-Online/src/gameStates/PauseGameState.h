#pragma once
#include "../engine/core/GameState.h"

class Window;
class GameObject;
class MovingObject;
class Collisions;
class Enemy;

class PauseGameState : public GameState
{
public:
	PauseGameState(GameState* pausedGameState);
	~PauseGameState() {};
	void render(Window* window) override;
	void unpauseState();
private:
	GameState* pauseGameState;
};