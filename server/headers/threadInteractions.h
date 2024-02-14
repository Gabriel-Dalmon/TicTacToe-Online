#pragma once

#include "socketRequirements.h"
#include "enum.h"
#include "threadInteractions.h"

// Empty function to avoid reading the Json::Value bridges so much that the WebThread have to wait to write in it
// This way to handle chnages in the bridge variables is too specific and kinda trash, it aspires to change for REAL alpha mal- uhh events
void doNothing() {};

// Swap the taskController function adress
void swapFunctionAddress(void (*taskController)());

// Packing function that creates and stores a Json::Value in the send-bridge for webThreads to send out
Json::Value packRequest(int reqType, void* reqData);

// Blocking a request for it not to be sent
void deactivateRequest(Json::Value* request);

// Read, treats, send back to the clients the new game or players state given a received request
// Switch case time. ¬ w ¬
void treatData();