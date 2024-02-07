#include "MP_PlayMoveGameState.h"

void MP_PlayMoveGameState::update(float deltaTime)
{
	GameState::update(deltaTime);
	m_ClientSocket->recvJSON();
}
