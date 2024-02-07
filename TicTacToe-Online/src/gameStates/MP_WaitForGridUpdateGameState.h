#pragma once
#include "../engine/core/MP_GameState.h"
#include "../objects/Grid.h"
#include "MP_WaitForGridUpdateGameState.h"
#include "../networking/ClientSocket.h"

class Window;
class GameObject;
class MP_WaitForGridUpdateGameState;

class MP_WaitForGridUpdateGameState : public MP_GameState
{
public:
	MP_PlayMoveGameState* m_pPlayGameState;

	MP_PlayMoveGameState();
	MP_PlayMoveGameState(MP_WaitForGridUpdateGameState previousGameState);
	~MP_PlayMoveGameState();
	void update(float deltaTime) override;

private:
	Grid* m_pGrid;
};