#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace Json {
	class Value;
}

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();
	const char* serializeToBytes(Json::Value jsonValue);
	Json::Value deserializeToJSON(char* pBytesValue);
	Json::Value recvJSON();
	void sendJSON(Json::Value);
	int connectSocket();
	void generateBuffer(Json::Value json);
protected:
	SOCKET m_ConnectSocket = INVALID_SOCKET;
	addrinfo* m_ServerAdressInfo = NULL;
	char* port;
};