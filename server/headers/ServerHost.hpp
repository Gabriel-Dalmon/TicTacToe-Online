#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <WS2tcpip.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6942"


class ServerHost {
public:
	ServerHost();
	~ServerHost();

	int host();
	int socketSetup();
	int reiceveFrom(SOCKET* client);
	int sendTo(SOCKET* client);
	void tick();

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket;
	SOCKET ClientSocket;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen;

	bool hosting;
	std::vector<SOCKET> clientList;

	int gridState[9];
};