
#include <utility>
#include <iostream>
#include <algorithm>


#include "socketRequirements.h"
#include "WritingThread.h"

// Empty Interface constru/destru
Thread::Thread() {
}
Thread::~Thread() { 
}

// WritingThread constru/destru (for WSA)
WritingThread::WritingThread()
{
    std::lock_guard<std::mutex> lock(initMutex);
    WSAStartup(MAKEWORD(2, 2), &m_wsaData);
}
WritingThread::~WritingThread()
{
    std::lock_guard<std::mutex> lock(initMutex);
    WSACleanup();
}


void WritingThread::RunThread() {
    MSG msg;
    DWORD Ret;

    // Initializing the window 
    std::pair<SOCKET, HWND> sockWinPair = WindowSocketInitialize(&m_wsaData);

    if (sockWinPair == std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL)) {
        // Setup caught an error, we kill the thread
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
