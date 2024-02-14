#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <utility>
#include <iostream>

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

enum RequestType
{
    IGNORE_REQ = 0, // Avoid sending request at all
    setName,
    makePlay
};


// Empty function to avoid reading the Json::Value bridges so much that the WebThread have to wait to write in it
// This way to handle chnages in the bridge variables is too specific and kinda trash, it aspires to change for REAL alpha mal- uhh events
void doNothing()
{
}
// Swap the taskController function adress
void swapFunctionAddress(void (*taskController)())
{
    if (taskController == doNothing)
        { taskController = treatData; }
    else
        { taskController = doNothing; }
}

// Packing function that creates and stores a Json::Value in the send-bridge for webThreads to send out
Json::Value packRequest(int reqType, void* reqData)
{
    Json::Value request;
    request["reqType"] = reqType;
    request["reqData"] = reqData;

    return request;
}
// Blocking a request for it not to be sent
void deactivateRequest(Json::Value* request)
{
    (*request)["reqType"] = IGNORE_REQ;
}

// Read, treats, send back to the clients the new game or players state given a received request
// Switch case time. ¬ w ¬
void treatData()
{
    // sw<itch case
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    // Creation of threads for communication
    Json::Value lastRecvReq[2];
    Json::Value lastSendReq[2];
    Thread webReceiver();
    Thread webSender();

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