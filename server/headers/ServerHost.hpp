#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6942"

// Header's sizes for fixed client-requests
#define DEFAULT_HEADSIZE 1


class ServerHost {
public:
	ServerHost();
	~ServerHost();

	// Network related functions
	int host();
	int socketSetup();
	int recieveFrom(SOCKET* client);
	int sendTo(SOCKET* client);

	// Game-logic related functions
	bool isMoveValid();

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket;
	std::vector<SOCKET> clientList;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen;

	bool hosting;

	int gridState[9];
};