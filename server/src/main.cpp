#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <utility>
#include <iostream>
#include <json/json.h>

#include "enum.h"
#include "socketRequirements.h"
#include "ReadingThread.h"
#include "WritingThread.h"
#include "threadInteractions.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    // Creation of threads for communication and the thread-bridge variables
    Json::Value lastRecvReq[2];
    Json::Value lastSendReq[2];
    ReadingThread webReceiver();
    WritingThread webSender();

    // Pointer to function (to avoid spam-reading lastRecvReq)
    void (*taskController)(Json::Value* recv[2], Json::Value* send[2], char* playersName[2], int* turnOf, int* grid[9], int* gameState) = emptyTreatData;

    // Intialize everything the server locally needs to keep track of players and the match
    char* playersName[2];
    int turnOf = 1;
    //bool noClients = false;
    int grid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int gameState = waiting_players;

    // Enter main server loop. If no clients are connected, a timer starts, and upon reaching XX, the server closes.
    while (true /*&& timer is below threashold*/)
    {
        taskController((Json::Value**)&lastRecvReq, (Json::Value**)&lastSendReq, &playersName[2], &turnOf, (int**)&grid[9], &gameState);
    }
}