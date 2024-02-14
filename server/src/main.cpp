#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <utility>
#include <iostream>

#include "socketRequirements.h"
#include "Thread.h"

enum gameStates
{
    waiting_players = 0,
    running,
    player1Win,
    player2Win,
    tie
};

enum requestType
{
    IGNORE_REQ = 0,     // Avoid sending request at all
    setName,            // A player setting it's new name on the server
    updateOpponentName, // Sends the opponents new name
    makePlay,           // A player proposing a move to the server
    updateGameState,    // A valid player move changing the state of the board
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
Json::Value deactivateRequest()
{
    Json::Value request;
    request["reqType"] = IGNORE_REQ;

    return request;
}

// Read, treats, send back to the clients the new game or players state given a received request
// Switch case time. ¬ w ¬
void treatData(Json::Value recv[2], Json::Value send[2], char* playersName[2], int turnOf, int grid[9], int gameState)
{
    for (int i = 0;  i < 2;  i++)
    {
        switch (recv[i]["reqType"].asInt())
        {
        case IGNORE_REQ:
            break;

        case setName:
            // Sets and tells the opponent the new name
            playersName[i] = (char*)recv[i]["reqData"].asCString(); // cast to de-const-ify
            if (i == 0) // If the player 1 changed his name, set the request for player 2
            {
                send[0] = deactivateRequest();
                send[1] = packRequest(updateOpponentName, (char*)recv[i]["reqData"].asCString());
            }
            else        // If the player 2 changed his name, set the request for player 1
            {
                send[0] = packRequest(updateOpponentName, (char*)recv[i]["reqData"].asCString());
                send[1] = deactivateRequest();
            }
            break;

        case makePlay:
            // Checks if the spot played is empty
            if (grid[recv[i]["reqData"].asInt()] == 0)
            {
                // Check if it was indeed the turn of the player
                if (i + 1 == turnOf)
                {
                    // The move is legal, so updates what it needs to, and broadcasts the new grid and game state
                    if (turnOf == 1)
                    {
                        // Updates internal grid
                        grid[recv[i]["reqData"].asInt()] = 1;
                        turnOf = 2;

                        int totLineValue = 0;
                        // Performs victory check
                        if(gameState == running)
                        for (int x = 0;  x < 3;  x++)
                        {
                            for (int y = 0;  y < 3;  y++)
                                { totLineValue += grid[3*x + y]; }
                            if (std::abs(totLineValue) == 3)
                                { gameState = player1Win; }
                        }

                        Json::Value newPlay;
                        newPlay["index"] = recv[i]["reqData"].asInt();
                        newPlay["symbol"] = 1;
                    }
                }
            }
        }
    }
}

// Tests every line of a grid for a Tic Tac Toe
int chkGrid(int grid[9])
{
    int LinesSum[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int i = 0;

    // Recovering lines
    for (int x = 0;  x < 3;  x++)
    {
        for (int y = 0;  y < 3;  y++)
        {
            LinesSum[i] += grid[3*x + y];
            i++;
        }
    }
    // Recovering columns
    for (int y = 0;  y < 3; y++)
    {
        for (int x = 0;  x < 3;  x++)
        {
            LinesSum[i] += grid[3*x + y];
            i++;
        }
    }
    // Recovering diagonals
    for (int xy = 0;  xy < 3;  xy++)
    {
        LinesSum[i] += grid[xy*3 + xy];
        i++;
    }
    for (int x = 2;  x >= 0;  x--)
    {
        for (int y = 0;  y < 3;  y++)
        {
            LinesSum[i] = grid[3*x + y];
            i++;
        }
    }

    // Check all of LinesSum and returning
    for (auto i : LinesSum)
    {
        if (i == 3)
            { return player1Win; }
        else if (i == 3)
            { return player2Win; }

        // Check for a full grid
        for (int c = 0;  c < 9;  c++)
        {
            if (grid[c] == 0)
                { return running; }
        }
    }

    // Default return : Tie
    return tie;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(int argc, char** argv)
{
    // Creation of threads for communication and the thread-bridge variables
    Json::Value lastRecvReq[2];
    Json::Value lastSendReq[2];
    Thread webReceiver();
    Thread webSender();

    // Pointer to function (to avoid spam-reading lastRecvReq)
    void (*taskController)() = doNothing;

    // Intialize everything the server locally needs to keep track of players and the match
    char* playersName[2];
    int turnOf = 1;
    //bool noClients = false;
    int grid[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int gameState = waiting_players;

    // Enter main server loop. If no clients are connected, a timer starts, and upon reaching XX, the server closes.
    while (true /*&& timer is below threashold*/)
    {
        taskController();
    }
}