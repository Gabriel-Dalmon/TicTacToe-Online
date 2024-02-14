#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <utility>
#include <iostream>

#include "socketRequirements.h"
#include "ReadingThread.h"
#include "WritingThread.h"
#include "enum.h"
#include "threadInteractions.h"



int main(int argc, char** argv)
{
    // Creation of threads for communication
    Json::Value lastRecvReq[2];
    Json::Value lastSendReq[2];
    ReadingThread webReceiver();
    WritingThread webSender();

    // Pointer to function (to avoid spam-reading lastRecvReq)
    void (*taskController)() = doNothing;

    // Intialize everything the server locally needs to keep track of players and the match
    int grid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char* playersName[2];
    SOCKET* turnOf;
    int matchState = waiting_players;
    bool noClients = false;

    // Enter main server loop. If no clients are connected, a timer starts, and upon reaching XX, the server closes.
    while (!noClients /*&& timer is below threashold*/)
    {
        taskController();
    }
}