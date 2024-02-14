
#include <utility>
#include <iostream>
#include <algorithm>


#include "socketRequirements.h"
#include "ReadingThread.h"


Thread::Thread() {
}

Thread::~Thread() {
    
}


void Thread::RunThread() {
    MSG msg;
    DWORD Ret;

    // Initializing the window 
    std::pair<SOCKET, HWND> sockWinPair = WindowSocketInitialize(&m_wsaData);

    if (sockWinPair == std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL)) {
        // Setupe caught an error, we kill the thread
        std::cerr << "error in thread inialzation" << std::endl;
        threadList.erase(std::remove(threadList.begin(), threadList.end(), m_This), threadList.end());
        return;
    };
    SOCKET Listen = sockWinPair.first;
    HWND Window = sockWinPair.second;


    while (Ret = GetMessage(&msg, Window, 0, 0)) {
        if (Ret != -1) {
            DispatchMessageW(&msg);
            //PostMessage(Window, (UINT)&msg, 0, 0);
        }
    }


    threadList.erase(std::remove(threadList.begin(), threadList.end(), m_This), threadList.end());
    return;
}
