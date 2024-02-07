#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WM_SOCKET WM_USER + 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>

#include "ServerHost.hpp"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6942"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ServerHost host;
	host.host();
	while (true) {
		host.tick();
	}
}

//int __cdecl main(void) {}

BOOL CALLBACK ServerWinProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)

{

    SOCKET Accept;



    switch (wMsg)

    {

    case WM_PAINT:

        // Process window paint messages

        break;

    case WM_SOCKET:

        // Determine whether an error occurred on the

        // socket by using the WSAGETSELECTERROR() macro

        if (WSAGETSELECTERROR(lParam))

        {

            // Display the error and close the socket

            closesocket((SOCKET)wParam);

            break;

        }

        // Determine what event occurred on the socket

        switch (WSAGETSELECTEVENT(lParam))

        {

        case FD_ACCEPT:

            // Accept an incoming connection

            Accept = accept(wParam, NULL, NULL);

            // Prepare accepted socket for read, write, and close notification

            WSAAsyncSelect(Accept, hDlg, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

            break;

        case FD_READ:

            // Receive data from the socket in wParam

            break;

        case FD_WRITE:

            // The socket in wParam is ready for sending data

            break;

        case FD_CLOSE:

            // The connection is now closed

            closesocket((SOCKET)wParam);

            break;

        }

        break;

    }

    return TRUE;

}