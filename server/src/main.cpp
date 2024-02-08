#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define WM_SOCKET WM_USER + 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_LOADSTRING 100


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

BOOL CALLBACK ServerWinProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wndclass;

    CHAR* ProviderClass = (CHAR*)"AsyncSelect";

    HWND Window;



    wndclass.style = CS_HREDRAW | CS_VREDRAW;

    wndclass.lpfnWndProc = (WNDPROC)ServerWinProc;

    wndclass.cbClsExtra = 0;

    wndclass.cbWndExtra = 0;

    wndclass.hInstance = NULL;

    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    wndclass.lpszMenuName = NULL;

    wndclass.lpszClassName = (LPCSTR)ProviderClass;



    if (RegisterClass(&wndclass) == 0)

    {

        printf("RegisterClass() failed with error %d\n", GetLastError());

        return NULL;

    }

    else

        printf("RegisterClass() is OK!\n");



    LPCSTR szTitle = (LPCSTR)"AsyncSelect";                 // Texte de la barre de titre
    LPCSTR szWindowClass = (LPCSTR)L"";            // nom de la classe de fenêtre principale
    Window = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(Window, SW_SHOW);
    UpdateWindow(Window);


	ServerHost host;
	host.host();
    WSAAsyncSelect(host.ListenSocket, Window, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
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