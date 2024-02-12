#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <utility>

#include "socketRequirements.h"
#include "Thread.h"



//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{

    Thread test;
    WSADATA wsaData;

    test.launchSocketThread(&wsaData);

    /*std::pair<SOCKET, HWND> sockWinPair = WindowSocketInitialize(&wsaData);

    if(sockWinPair == std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL)) {
        return 1;
    };

    SOCKET Listen = sockWinPair.first;
    HWND Window = sockWinPair.second;*/
    while (true) {
        //
    }

}