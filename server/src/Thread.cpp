
#include <utility>
#include <iostream>
#include <algorithm>


#include "socketRequirements.h"
#include "Thread.h"


std::vector<HANDLE> Thread::threadList = {};

Thread::Thread() {
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread::SummonThread, (void*)this, 0, NULL);
}

Thread::~Thread() {
    
}

void Thread::SummonThread(void* Instance) {
    Thread* This = (Thread*)Instance;
    This->m_This = (HANDLE)Instance;
    threadList.push_back(This->m_This);
    This->RunSocketThread();
}


void Thread::RunSocketThread() {
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
