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
    This->RunThread();
}
