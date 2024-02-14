#include "socketRequirements.h"
#include "enum.h"
#include "threadInteractions.h"


void swapFunctionAddress(void (*taskController)())
{
    if (taskController == doNothing)
    {
        taskController = treatData;
    }
    else
    {
        taskController = doNothing;
    }
}


Json::Value packRequest(int reqType, void* reqData)
{
    Json::Value request;
    request["reqType"] = reqType;
    request["reqData"] = reqData;

    return request;
}


void deactivateRequest(Json::Value* request)
{
    (*request)["reqType"] = IGNORE_REQ;
}


void treatData()
{
    // sw<itch case
}