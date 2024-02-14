#pragma once

#include "enum.h"
#include "socketRequirements.h"
#include "threadInteractions.h"

// Empty function to avoid reading the Json::Value bridges so much that the WebThread have to wait to write in it
// This way to handle chnages in the bridge variables is too specific and kinda trash, it aspires to change for REAL alpha mal- uhh events
void emptyTreatData(Json::Value* recv[2], Json::Value* send[2], char* playersName[2], int* turnOf, int* grid[9], int* gameState) {};

// Read, treats, send back to the clients the new game or players state given a received request
// Switch case time. ¬ w ¬
void treatData(Json::Value* recv[2], Json::Value* send[2], char* playersName[2], int* turnOf, int* grid[9], int* gameState);

// Tests every line of a grid for a Tic Tac Toe
int chkGrid(int grid[9]);

// Swap the taskController function adress
void swapFunctionAddress(void (*taskController)());

// Packing function that creates and stores a Json::Value in the send-bridge for webThreads to send out
Json::Value packRequest(int reqType, void* reqData);

// Blocking a request for it not to be sent
Json::Value deactivateRequest();