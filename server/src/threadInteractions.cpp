#include "socketRequirements.h"
#include "enum.h"
#include "threadInteractions.h"


void swapFunctionAddress(void (*taskController)(Json::Value* recv[2], Json::Value* send[2], char* playersName[2], int* turnOf, int* grid[9], int* gameState))
{
    if (taskController == emptyTreatData)
    {
        taskController = treatData;
    }
    else
    {
        taskController = emptyTreatData;
    }
}


Json::Value packRequest(int reqType, void* reqData)
{
    Json::Value request;
    request["reqType"] = reqType;
    request["reqData"] = reqData;

    return request;
}


Json::Value deactivateRequest()
{
    Json::Value request;
    request["reqType"] = IGNORE_REQ;

    return request;
}


// Read, treats, send back to the clients the new game or players state given a received request
// Switch case time. ¬ w ¬
void treatData(Json::Value* recv[2], Json::Value* send[2], char* playersName[2], int* turnOf, int* grid[9], int* gameState)
{
    for (int i = 0; i < 2; i++)
    {
        switch ((*recv[i])["reqType"].asInt())
        {
        case IGNORE_REQ:
            break;

        case setName:
            // Sets and tells the opponent the new name
            playersName[i] = (char*)(*recv)[i]["reqData"].asCString(); // cast to de-const-ify
            if (i == 0) // If the player 1 changed his name, set the request for player 2
            {
                *send[0] = deactivateRequest();
                *send[1] = packRequest(updateOpponentName, (char*)(*recv)[i]["reqData"].asCString());
            }
            else        // If the player 2 changed his name, set the request for player 1
            {
                *send[0] = packRequest(updateOpponentName, (char*)(*recv)[i]["reqData"].asCString());
                *send[1] = deactivateRequest();
            }
            break;

        case makePlay:
            // Checks if the spot played is empty, and that it was indeed the player's turn
            if (grid[(*recv)[i]["reqData"].asInt()] == 0 && i + 1 == *turnOf)
            {
                // The move is legal, so updates what it needs to, and broadcasts the new grid and game state
                if (*turnOf == 1)
                {
                    // Updates internal grid
                    *grid[(*recv)[i]["reqData"].asInt()] = 1;
                    *turnOf = 2;
                }
                else
                {
                    // Updates internal grid
                    *grid[(*recv)[i]["reqData"].asInt()] = -1;
                    *turnOf = 1;
                }

                // Updates game state
                *gameState = chkGrid(*grid);

                // Prepares the new move to send out
                Json::Value newPlay;
                newPlay["index"] = (*recv)[i]["reqData"].asInt();
                newPlay["symbol"] = 1;
                newPlay["gameState"] = *gameState;

                // Broadcasts the newPlay
                *send[0] = packRequest(updateMatchState, (void*)&newPlay);
                *send[1] = packRequest(updateMatchState, (void*)&newPlay);
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
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            LinesSum[i] += grid[3 * x + y];
            i++;
        }
    }
    // Recovering columns
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            LinesSum[i] += grid[3 * x + y];
            i++;
        }
    }
    // Recovering diagonals
    for (int xy = 0; xy < 3; xy++)
    {
        LinesSum[i] += grid[xy * 3 + xy];
        i++;
    }
    for (int x = 2; x >= 0; x--)
    {
        for (int y = 0; y < 3; y++)
        {
            LinesSum[i] = grid[3 * x + y];
            i++;
        }
    }

    // Check all of LinesSum and returning
    for (auto i : LinesSum)
    {
        if (i == 3)
        {
            return player1Win;
        }
        else if (i == 3)
        {
            return player2Win;
        }

        // Check for a full grid
        for (int c = 0; c < 9; c++)
        {
            if (grid[c] == 0)
            {
                return running;
            }
        }
    }

    // Default return : Tie
    return tie;
}