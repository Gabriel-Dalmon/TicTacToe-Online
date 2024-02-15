#include "Thread.h"

void Thread::Run()
{
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread::EnterThread, (void*)this, 0, NULL);
}

void Thread::EnterThread(void* threadAddress) {
    Thread* threadInstance = (Thread*)threadAddress;
    threadInstance->InitThread();
    threadInstance->MainLoop();
}