#pragma once
#include "../engine/core/MP_GameState.h"
#include "../networking/ClientSocket.h"

class Window;
class GameObject;
class MP_PlayMoveGameState;
class MP_WaitForGridUpdateGameState;

class MP_InitSocketGameState : public MP_GameState
{
public:
	MP_InitSocketGameState();
	MP_InitSocketGameState(Window* pWindow);
	~MP_InitSocketGameState();
	void initFutureGameStates();
	void update(float deltaTime) override;

private:
	ClientSocket* m_ClientSocket;
	MP_PlayMoveGameState* m_pPlayMoveGameState;
	MP_WaitForGridUpdateGameState* m_pWaitForGridUpdateGameState;
};