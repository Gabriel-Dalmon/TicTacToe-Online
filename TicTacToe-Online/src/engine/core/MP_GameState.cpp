#include "MP_GameState.h"
#include "../../networking/ClientSocket.h"

void MP_GameState::sendJSONWrapper(Json::Value json)
{
	m_ClientSocket->sendJSON(json);
}
