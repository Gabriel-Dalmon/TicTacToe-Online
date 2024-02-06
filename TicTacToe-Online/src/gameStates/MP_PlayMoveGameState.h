#pragma once
#pragma once
#include "../engine/core/GameState.h"

class Window;
class GameObject;

class MP_PlayMoveGameState : public GameState
{
public:
	MP_PlayMoveGameState();
	MP_PlayMoveGameState(MP_WaitForGridUpdateGameState previousGameState);
	~MP_PlayMoveGameState();
	void initPlayGameState(Window* pWindow);
	void enter() override;
	void update(float deltaTime) override;

private:
	SOCKET* socket;
};