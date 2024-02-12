
#include <utility>
#include <iostream>

#include "Thread.h"
#include "socketRequirements.h"

int Thread::threadNumber = 0;
HANDLE Thread::threadList[8] = {NULL};

Thread::Thread() {
    
}

Thread::~Thread() {
    //threadList[m_ID].remove();
}

void Thread::SummonThread(void* Instance) {
    /*HANDLE threadAddr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Thread::SocketThread), NULL, 0, NULL);
	 threadNumber++;
	 threadList[threadNumber] = threadAddr;*/
    Thread* This = (Thread*)Instance;
    This->SocketThread();
}

void Thread::launchSocketThread(LPWSADATA wsaDataAddr) {
    m_ID = threadNumber;
    threadList[threadNumber] = this;
    threadNumber++;
    m_wsaDataAddr = wsaDataAddr;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread::SummonThread, (void*)this, 0, NULL);
}

void Thread::SocketThread() {
    std::cout << "esogijn";
    bool threadAlive = TRUE;

    std::pair<SOCKET, HWND> sockWinPair = WindowSocketInitialize(m_wsaDataAddr);

    if (sockWinPair == std::pair<SOCKET, HWND>(INVALID_SOCKET, NULL)) {
        // Setupe caught an error, we kill the thread
        std::cerr << "error in thread inialzation" << std::endl;
        delete this;
        return;
    };

    SOCKET Listen = sockWinPair.first;
    HWND Window = sockWinPair.second;

    while (threadList[m_ID] == this) {
        // Thread is alive
    }

    delete this;
}