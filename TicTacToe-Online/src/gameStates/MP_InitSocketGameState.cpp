#include "MP_InitSocketGameState.h"
#include "MP_PlayMoveGameState.h"
#include "../engine/core/GameManager.h"

MP_InitSocketGameState::MP_InitSocketGameState(Window* pWindow)
{
	m_ClientSocket = new ClientSocket("fsqef", "fsdfg");
	this->initFutureGameStates();
}

void MP_InitSocketGameState::initFutureGameStates()
{
	m_pPlayMoveGameState = new MP_PlayMoveGameState(m_ClientSocket, );
	m_pWaitForGridUpdateGameState = new MP_WaitForGridUpdateGameState(m_ClientSocket, );

	// Binding the GameStates to each other to allow them to switch between each others.
	m_pPlayMoveGameState->m_pWaitGameState = m_pWaitForGridUpdateGameState;
	m_pWaitForGridUpdateGameState->m_pPlayGameState = m_pPlayMoveGameState;
}


void MP_InitSocketGameState::update(float deltaTime)
{
	Json::Value oData = m_ClientSocket->recvJSON();
	if (oData) {
		if (oData["start"] == true) {
			GameManager::instance().switchGameState(m_pPlayMoveGameState);
		}
		else {
			GameManager::instance().switchGameState(m_pWaitForGridUpdateGameState);
		}
	}
}