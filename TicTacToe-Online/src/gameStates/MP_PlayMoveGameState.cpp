#include "MP_PlayMoveGameState.h"
#include "../engine/core/GameManager.h"

void MP_PlayMoveGameState::update(float deltaTime)
{
	GameState::update(deltaTime);
	Json::Value data = m_ClientSocket->recvJSON();
	if (data) {
		GameManager::instance().switchGameState();
	}
}
