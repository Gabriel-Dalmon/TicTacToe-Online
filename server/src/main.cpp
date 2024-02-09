#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <conio.h>

#include "ServerHost.hpp"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6942"

enum RequestType
{
	setName = 0,
	makePlay
};

int __cdecl main(void)
{
	ServerHost host;
	host.host();

	while (true)
	{
		host.recieveFrom(&host.ListenSocket);
	}

	// Asks for where the player want to play
	printf("\n\nWanna break from the ads ? ");
	char input;
	input = _getch();
}