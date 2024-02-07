#pragma once

#include "GameState.h"
#include <json/json.h>

class ClientSocket;

class MP_GameState : public GameState
{
public:
	void sendJSONWrapper(Json::Value json);
protected:
	ClientSocket* m_ClientSocket;
};