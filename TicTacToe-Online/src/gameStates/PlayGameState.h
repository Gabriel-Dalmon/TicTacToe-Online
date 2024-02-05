#pragma once
#include "../engine/core/GameState.h"

class Window;
class GameObject;

class PlayGameState : public GameState 
{
public:
	PlayGameState();
	PlayGameState(Window* pWindow);
	~PlayGameState();
	void initGrid(Window* pWindow);
	void switchStateDefeat();
	void switchStatePause();
	void enter() override;
	void update(float deltaTime) override;

private:

};