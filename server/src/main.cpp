#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <utility>

#include "socketRequirements.h"
#include "Thread.h"

enum matchStates
{
    waiting_players = 0,
    running,
    player1Win,
    player2Win,
    tie
};


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
    while (true)
    {
        // Intialize everything the server locally needs to keep track of players and the match
        int grid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        char* playersName[2];
        SOCKET* turnOf;
        int matchState = waiting_players;
        bool noClients = false;

        // Enter main server loop. If no clients are connected, a timer starts, and upon reaching XX, the server closes.
        while (!noClients /*&& timer is below threashold*/)
        {   
            // By default, it will do nothing until it receives an event
        }
            
    }

}