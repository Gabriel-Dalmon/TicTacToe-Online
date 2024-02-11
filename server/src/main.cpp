#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <utility>

#include "socketRequirements.hpp"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    WSADATA wsaData;

    std::pair<SOCKET, HWND> sockWinPair = WindowSocketInitialize(&wsaData);

    if(sockWinPair == std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL)) {
        return 1;
    };

    SOCKET Listen = sockWinPair.first;
    HWND Window = sockWinPair.second;

}
